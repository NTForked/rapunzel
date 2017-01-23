/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                                 Hair Object
/-------------------------------------------------------------------------------
*/

#include "Hair.h"

// Constructor.
Hair::Hair( HairType _type, HairStyle _style, unsigned int _length, std::string _runsOn, std::string HAIR_OFF ):Object3D(){

    
    // Declarations.
    unsigned int i, remainder;
    Vector3D aux;

    // OFF specific.
    type = _type;
    style = _style;
    scale = Vector3D( 100, 100, 100 );
    wind = false;
    windDirection = X;
    gravity = NORMAL_GRAVITY;
    delta = 0.0f;
    length = _length;
    runsOn = _runsOn;
    off = new offLoader( StraightForward );

    switch( type ){
    case HumanHair:
        if( style == Custom )
            off->readOFF( SCALP_OFF );
        else
            off->readOFF( HAIR_OFF );
        break;
    case AnimalFur:
        if( style == Custom )
            off->readOFF( SKIN_OFF );
        else
            off->readOFF( FUR_OFF );
        break;
    default:
        break;
    }
    vertices.resize( off->finalVertices.size() );
    vertices = off->finalVertices;
    normals.resize( off->normals.size() );
    normals = off->normals;
    vertices.resize( ( length + 1 ) * vertices.size() );
    distancesUpToCenter = std::vector<float>();

    // Creating scarce buffer with hair base.
    for( i = vertices.size() / ( length + 1 ) + 1; i < vertices.size(); i += 1 ){
        if( i % ( length + 1 ) == 0 )
            vertices[i] = vertices[i / ( length + 1 )];
    }
    for( i = length + 1; i < vertices.size() / ( length + 1 ); i += 1 ){
        if( i % ( length + 1 ) == 0 )
            vertices[i] = vertices[i / ( length + 1 )];
    }
    computeCenterOfGravity();
    downsizedCenterOfGravity = centerOfGravity;
    downsizedCenterOfGravity.y -= Y_DOWNSIZE_OFFSET;
    upsizedCenterOfGravity = centerOfGravity;
    upsizedCenterOfGravity.y += Y_DOWNSIZE_OFFSET;
    // Expanding base hairs away from center.
    for( i = 0; i < vertices.size(); i += 1 )
        if( i % ( length + 1 ) == 0 )
            vertices[i] += ( vertices[i] - centerOfGravity ) * 0.1f;
    //*/
    // Populating the entire buffer
    for( i = 0; i < vertices.size(); i += 1 ){
        remainder = i % ( length + 1 );
        if( remainder != 0 ){
            vertices[i] = vertices[i - 1] + ( vertices[i - remainder] - centerOfGravity ).normalize() * HAIR_STRAND_LENGTH;
        }
    }
    previousVertices.resize( vertices.size() );
    for( i = 0; i < vertices.size(); i += 1 ){
        previousVertices[i] = vertices[i];
    }

    distancesUpToCenter.resize( vertices.size() / ( length + 1 ) );
    for( i = 0; i < vertices.size(); i += 1 )
        if( i % ( length + 1 ) == 0 ){
            distancesUpToCenter[i / ( length + 1 )] = vertices[i].Distance( centerOfGravity ) + COLLISION_PADDING_OFFSET;
        }

    for( i = 1; i < vertices.size(); i += 1 )
        if( i % ( length + 1 ) != 0 ){
            indices.push_back( i - 1 );
            indices.push_back( i );
        }

    if( length == 0 ){
        vertices.resize( 2 );
        indices.resize( 2 );
        vertices[0] = Vector3D( 0, 0, 0 );
        vertices[1] = Vector3D( 0, 0, 0 );
        indices[0] = 0;
        indices[1] = 1;
    }

    // VBO generation.
    if( glIsBufferARB( vbo ) )
        glDeleteBuffersARB( 1, &vbo );
    glGenBuffersARB( 1, &vbo );
    glBindBufferARB( GL_ARRAY_BUFFER, vbo );
    glBufferDataARB( GL_ARRAY_BUFFER, vertices.size() * sizeof( Vector3D ), &vertices[0], GL_DYNAMIC_DRAW );
    glBindBufferARB( GL_ARRAY_BUFFER, 0 );
    
    if( glIsBuffer( ibo ) )
        glDeleteBuffersARB( 1, &ibo );
    glGenBuffersARB( 1, &ibo );
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, ibo );
    glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( unsigned int ), &indices[0], GL_DYNAMIC_DRAW );
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void Hair::tiltForward( float _rotation ){
    // Declarations.
    unsigned int i;
    float cosine, sine, y, z;

    cosine = (float)cos( _rotation * M_PI / 180.0f );
    sine = (float)sin( _rotation * M_PI / 180.0f );
    for( i = 0; i < vertices.size(); i += ( length + 1 ) ){
        y = vertices[i].y;
        z = vertices[i].z;
        vertices[i].y = cosine    * y + sine        * z;
        vertices[i].z = -sine    * y + cosine    * z;
    }
}

void Hair::tiltSideways( float _rotation ){
    // Declarations.
    unsigned int i;
    float cosine, sine, x, y;

    cosine = (float)cos( _rotation * M_PI / 180.0f );
    sine = (float)sin( _rotation * M_PI / 180.0f );
    for( i = 0; i < vertices.size(); i += ( length + 1 ) ){
        x = vertices[i].x;
        y = vertices[i].y;
        vertices[i].x = cosine    * x + sine        * y;
        vertices[i].y = -sine    * x + cosine    * y;
    }
}

Vector3D Hair::getNextVertex( Vector3D oldVertex, Vector3D centerOfGravity, float topHeight, unsigned int iteration  ){

    // Declarations.
    Vector3D newVertex;

    switch( style ){
        case StraightUp:
            newVertex.x = oldVertex.x;
            newVertex.y = oldVertex.y + 0.1f;
            newVertex.z = oldVertex.z;
            break;
        case Locks:
            newVertex.x = oldVertex.x + ( oldVertex.x - centerOfGravity.x ) / 5;
            newVertex.y = oldVertex.y + ( oldVertex.y - centerOfGravity.y ) / 5;
            newVertex.z = oldVertex.z + ( oldVertex.z - centerOfGravity.z ) / 5;
            break;
        case OverHead:
            newVertex.x = oldVertex.x + ( oldVertex.x - centerOfGravity.x ) / 5;
            newVertex.y = oldVertex.y + ( oldVertex.y - centerOfGravity.y ) / 5;
            newVertex.z = oldVertex.z + ( oldVertex.z - centerOfGravity.z ) / 5 - 0.1f;
            break;
    }
    //if( newVertex.y > topHeight )
    //    newVertex.y = topHeight - 5;
    //else{
        //vertex.y -= 1;
        // TODO: implement topHeight - vertex.y dependence
    //}
    return newVertex;
}

void Hair::computeCenterOfGravity(){

    // Declarations.
    float smallestX, greatestX, smallestY, smallestZ, greatestZ;
    unsigned int i;

    // Initialisations.
    smallestX = INF_DIM;
    smallestY = INF_DIM;
    smallestZ = INF_DIM;
    greatestX = -INF_DIM;
    greatestY = -INF_DIM;
    greatestZ = -INF_DIM;

    for( i = 0; i < vertices.size(); i += 1 ){
        smallestX = std::min( smallestX, vertices[i].x );
        smallestY = std::min( smallestY, vertices[i].y );
        smallestZ = std::min( smallestZ, vertices[i].z );
        greatestX = std::max( greatestX, vertices[i].x );
        greatestY = std::max( greatestY, vertices[i].y );
        greatestZ = std::max( greatestZ, vertices[i].z );
    }
    centerOfGravity.x = ( smallestX + greatestX ) / 2;
    centerOfGravity.y = ( smallestY + greatestY ) / 2;
    centerOfGravity.z = ( smallestZ + greatestZ ) / 2;
}

void Hair::computeCustomDraw(){

    unsigned int i, l;
    std::vector<Vector3D> aux, normals_aux, finalVertices, finalNormals;
    
    finalVertices = std::vector<Vector3D>();
    finalNormals = std::vector<Vector3D>();

    for( i = 0; i < vertices.size(); i += 1 ){
        for( l = 0; l < length; l += 1 ){
            finalVertices.push_back( Vector3D( O ) );
        }
    }

    for( i = 0; i < vertices.size(); i += 1 ){
        finalVertices( i, 0 ) = vertices[i];
    }

    for( i = 0; i < vertices.size(); i += 1 ){
        for( l = 1; l < length; l += 1 ){
            finalVertices( i, l ) = getNextVertex( finalVertices( i, l - 1 ), centerOfGravity, greatestY + HAIR_HEIGHT, l );
        }
    }
    for( i = 0; i < finalVertices.size() / length; i += 3 ){
        for( l = 1; l < length; l += 1 ){
            /* Base. Optional.
            aux.push_back( finalVertices( i, l ) );
            aux.push_back( finalVertices( i + 1, l ) );
            aux.push_back( finalVertices( i + 2, l ) );
            finalNormals.push_back( computeNormal( finalVertices( i, l ), finalVertices( i + 1, l ), finalVertices( i + 2, l ) ) );
            //*/
            aux.push_back( finalVertices( i, l ) );
            aux.push_back( finalVertices( i, l - 1 ) );
            aux.push_back( finalVertices( i + 2, l - 1 ) );
            finalNormals.push_back( computeNormal( finalVertices( i, l ), finalVertices( i, l - 1 ), finalVertices( i + 2, l - 1 ) ) );

            aux.push_back( finalVertices( i + 2, l ) );
            aux.push_back( finalVertices( i, l ) );
            aux.push_back( finalVertices( i + 2, l ) );
            finalNormals.push_back( computeNormal( finalVertices( i + 2, l ), finalVertices( i, l ), finalVertices( i + 2, l ) ) );

            aux.push_back( finalVertices( i + 2, l ) );
            aux.push_back( finalVertices( i + 2, l - 1 ) );
            aux.push_back( finalVertices( i + 1, l - 1 ) );
            finalNormals.push_back( computeNormal( finalVertices( i + 2, l ), finalVertices( i + 2, l - 1 ), finalVertices( i + 1, l - 1 ) ) );

            aux.push_back( finalVertices( i + 1, l ) );
            aux.push_back( finalVertices( i + 2, l ) );
            aux.push_back( finalVertices( i + 1, l - 1 ) );
            finalNormals.push_back( computeNormal( finalVertices( i + 1, l ), finalVertices( i + 2, l ), finalVertices( i + 1, l - 1 ) ) );

            aux.push_back( finalVertices( i + 1, l ) );
            aux.push_back( finalVertices( i + 1, l - 1 ) );
            aux.push_back( finalVertices( i, l ) );
            finalNormals.push_back( computeNormal( finalVertices( i + 1, l ), finalVertices( i + 1, l - 1 ), finalVertices( i, l ) ) );

            aux.push_back( finalVertices( i, l ) );
            aux.push_back( finalVertices( i + 1, l - 1 ) );
            aux.push_back( finalVertices( i, l - 1 ) );
            finalNormals.push_back( computeNormal( finalVertices( i, l ), finalVertices( i + 1, l - 1 ), finalVertices( i, l - 1 ) ) );
        }
    }
    finalVertices = aux;
    vertices = finalVertices;
    normals = finalNormals;
}

void Hair::draw( Vector3D color ){
    
    if( !visible )
        return;
    
    glDisable( GL_COLOR_MATERIAL );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

    // Rotation.
    glRotatef( rotation.x , 1.0 , 0.0 , 0.0 );
    glRotatef( rotation.y , 0.0 , 1.0 , 0.0 );
    glRotatef( rotation.z , 0.0 , 0.0 , 1.0 );

    // Translation.
    glTranslatef( translation.x , translation.y , translation.z );

    // Scale.
    glScalef( scale.x, scale.y, scale.z );

    // Material settings.
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, Vector4D( color.x, color.y, color.z, 1 ).Array() );
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, Vector4D( color.x, color.y, color.z, 1 ).Array() );
    // glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, Vector4D( HAIR_COLOR, 1 ).Array() );
    
    // Color.
    glColor3f( color.x, color.y, color.z );
    
    // VBO update and draw.
    glBindBufferARB( GL_ARRAY_BUFFER, vbo );
    if( runsOn == "CPU" )
        glBufferDataARB( GL_ARRAY_BUFFER, vertices.size() * sizeof( Vector3D ), &vertices[0], GL_DYNAMIC_DRAW );
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, NULL );

    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, ibo );

    glDrawElements( GL_LINES, indices.size(), GL_UNSIGNED_INT, 0 );

    glDisableClientState( GL_VERTEX_ARRAY );
    glBindBufferARB( GL_ARRAY_BUFFER, 0 );
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, 0 );
    //*/
    
    /* Traditional draw.
    unsigned int i;
    glBegin( GL_LINES );
    for( i = 0; i < vertices.size(); i += 1 ){
        if( i % ( length + 1 ) != 0 ){
            glVertex3f( vertices[i - 1].x, vertices[i - 1].y, vertices[i - 1].z );
            glVertex3f( vertices[i].x, vertices[i].y, vertices[i].z );
        }
    }
    glEnd();
    //*/
    glPopMatrix();
    glEnable( GL_COLOR_MATERIAL );
}

void Hair::draw(){
    
    if( !visible )
        return;
    
    glDisable( GL_COLOR_MATERIAL );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

    // Rotation.
    glRotatef( rotation.x , 1.0 , 0.0 , 0.0 );
    glRotatef( rotation.y , 0.0 , 1.0 , 0.0 );
    glRotatef( rotation.z , 0.0 , 0.0 , 1.0 );

    // Translation.
    glTranslatef( translation.x , translation.y , translation.z );

    // Scale.
    glScalef( scale.x, scale.y, scale.z );

    // Material settings.
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, Vector4D( HAIR_COLOR, 1 ).Array() );
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, Vector4D( HAIR_COLOR, 1 ).Array() );
    // glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, Vector4D( HAIR_COLOR, 1 ).Array() );
    
    // Color.
    glColor3f( HAIR_COLOR );
    
    // VBO update and draw.
    glBindBufferARB( GL_ARRAY_BUFFER, vbo );
    if( runsOn == "CPU" )
        glBufferDataARB( GL_ARRAY_BUFFER, vertices.size() * sizeof( Vector3D ), &vertices[0], GL_DYNAMIC_DRAW );
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, NULL );

    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, ibo );

    glDrawElements( GL_LINES, indices.size(), GL_UNSIGNED_INT, 0 );

    glDisableClientState( GL_VERTEX_ARRAY );
    glBindBufferARB( GL_ARRAY_BUFFER, 0 );
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, 0 );
    //*/
    
    /* Traditional draw.
    unsigned int i;
    glBegin( GL_LINES );
    for( i = 0; i < vertices.size(); i += 1 ){
        if( i % ( length + 1 ) != 0 ){
            glVertex3f( vertices[i - 1].x, vertices[i - 1].y, vertices[i - 1].z );
            glVertex3f( vertices[i].x, vertices[i].y, vertices[i].z );
        }
    }
    glEnd();
    //*/
    glPopMatrix();
    glEnable( GL_COLOR_MATERIAL );
}

float Hair::collisionOffset( Vector3D vertex, Vector3D referenceVertex ){
    return referenceVertex.Distance( centerOfGravity ) - vertex.Distance( centerOfGravity );
}

float Hair::collisionOffsetAfterFall( Vector3D vertex, Vector3D referenceVertex, float fallSpeed ){
    return referenceVertex.Distance( centerOfGravity ) - ( vertex - Vector3D( 0.0f, fallSpeed, 0.0f ) ).Distance( centerOfGravity );
}

float Hair::collisionOffsetAfterWind( Vector3D vertex, Vector3D referenceVertex, float windSpeed ){
    return referenceVertex.Distance( centerOfGravity ) - ( vertex + windDirection * windSpeed ).Distance( centerOfGravity );
}

void Hair::restoreFromCollision( Vector3D &vertex, Vector3D previousVertex, Vector3D referenceVertex ){
    // Declarations.
    float collisionOffset_v;
    
    // Collision with mannequin.
    collisionOffset_v = collisionOffset( vertex, referenceVertex );
    if( collisionOffset_v > 0 ){
        vertex += ( vertex - centerOfGravity ).normalize() * ( collisionOffset_v + RESTORATION_OFFSET );
    }

    // If real inter-node hair length surpasses than permitted length
    if( vertex.Distance( previousVertex ) > HAIR_STRAND_LENGTH * HAIR_ELASTICITY_FACTOR ){
        // Restore it to normal length.
        vertex = previousVertex + ( vertex - previousVertex ).normalize() * HAIR_STRAND_LENGTH;
    }
}

void Hair::fall( Vector3D &vertex, Vector3D copy, Vector3D referenceVertex, float fallSpeed ){
    // If it's standing up    
    if( vertex.y > referenceVertex.y ){
        // Get it away from the mannequin.
        vertex += ( copy - upsizedCenterOfGravity ).normalize() * fallSpeed;
    }
    // If it's hanging down
    else{
        // Bring it closer to the mannequin.
        vertex += ( downsizedCenterOfGravity - copy ).normalize() * fallSpeed;
    }
    vertex.y -= fallSpeed;
}

void Hair::negativeGravity( Vector3D &vertex, Vector3D copy, Vector3D referenceVertex, float fallSpeed ){
    // If it's hanging down
    if( vertex.y < referenceVertex.y ){
        // Get it away from the mannequin.
        vertex += ( copy - downsizedCenterOfGravity ).normalize() * fallSpeed;
    }
    // If it's standing up
    else{
        // Bring it closer to the mannequin.
        vertex += ( upsizedCenterOfGravity - copy ).normalize() * fallSpeed;
    }
    // Let it go up.
    vertex.y += fallSpeed;
}

void Hair::blowWind( Vector3D &vertex, Vector3D previousVertex, Vector3D previousPreviousVertex, float windSpeed, float t, float t0 ){
    // Linear.
    vertex += windDirection * windSpeed;

    // Sinusoidal.
    // vertex += windDirection * abs( sin( t ) ) * HAIR_WAVE_LENGTH * windSpeed;

    // Euler. Not functional.
    // vertex = previousVertex + windDirection * windSpeed * t;

    // Verlet. Not functional.
    // vertex = ( previousVertex - previousPreviousVertex + windDirection * t * t ) * windSpeed;
}

void Hair::simulateAnimation(){

    // Declarations.
    unsigned int i, remainder;
    Vector3D referenceVertex, previousVertex;
    std::vector< Vector3D > copy;

    copy = vertices;

    for( i = 1; i < vertices.size(); i += 1 ){
        remainder = i % ( length + 1 );
        if( remainder != 0 ){ // if it's not a base hair
            if( gravity == NORMAL_GRAVITY )
                fall( vertices[i], copy[i], copy[i - remainder], FALL_SPEED );
            else if( gravity == NEGATIVE_GRAVITY )
                negativeGravity( vertices[i], copy[i], copy[i - remainder], FALL_SPEED );
            if( wind ){
//                delta += DELTA_INCREMENT;
                blowWind( vertices[i], vertices[i - 1], copy[i - remainder], WIND_SPEED * (float)( rand() % 100 ) / 100.0f, timeSinceLastSimulation, 0 );
//                if( stuck( vertices[i], previousVertices[i] ) )
//                    vertices[i] += ( vertices[i] - centerOfGravity ).normalize() * 0.01f;
            }
            restoreFromCollision( vertices[i], vertices[i - 1], vertices[i - remainder] );
        //    previousVertices[i] = vertices[i];
        }
    }
}

bool Hair::stuck( Vector3D vertex, Vector3D previousVertex ){
    if( vertex == previousVertex )
        return true;
    return false;
}
