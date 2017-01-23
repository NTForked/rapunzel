/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                               Mannequin Object
/-------------------------------------------------------------------------------
*/

#include "Mannequin.h"

// Constructor.
Mannequin::Mannequin( MannequinType _type ):Object3D(){
    
    // Declarations.
    unsigned int i;

    // Default values.
    type = _type;
    scale = Vector3D( 100, 100, 100 );
    rotation = Vector3D( 0, 0, 0 );
    off = new offLoader( StraightForward );
    switch( type ){
    case Human:
        off->readOFF( MANNEQUIN_OFF );
        break;
    case Animal:
        off->readOFF( ANIMAL_OFF );
        break;
    default:
        break;
    }
    vertices = off->finalVertices;
    normals = off->normals;
    computeCenterOfGravity();
    
    /* For "WithFaces".
    for( i = 0; i < vertices.size(); i += 1 ){
        indices.push_back( i );
    }
    //*/
    // For "StraightForward".
    for( i = 0; i < off->faces.size(); i += 3 ){
        indices.push_back( off->faces[i] );
        indices.push_back( off->faces[i + 1] );
        indices.push_back( off->faces[i + 2] );
    }
    /* Convert normal-per-face to normal-per-pixel.
    normals.resize( normals.size() * 3 );
    for( i = normals.size() / 3 - 1; i > 0; i -= 1 ){
        normals[i * 3] = normals[i];
    }
    unsigned int remainder;
    for( i = 1; i < normals.size(); i += 1 ){
        remainder = i % 3;
        if( remainder != 0 ){
            normals[i] = normals[i - remainder];
        }
    }
    //*/

    // VBO generation.
    if( glIsBufferARB( vbo ) )
        glDeleteBuffersARB( 1, &vbo );
    glGenBuffersARB( 1, &vbo );
    glBindBufferARB( GL_ARRAY_BUFFER, vbo );
    glBufferDataARB( GL_ARRAY_BUFFER, vertices.size() * sizeof( Vector3D ), &vertices[0], GL_STATIC_DRAW );
    glBindBufferARB( GL_ARRAY_BUFFER, 0 );
    
    if( glIsBufferARB( nbo ) )
        glDeleteBuffersARB( 1, &nbo );
    glGenBuffersARB( 1, &nbo );
    glBindBufferARB( GL_ARRAY_BUFFER, nbo );
    glBufferDataARB( GL_ARRAY_BUFFER, normals.size() * sizeof( Vector3D ), &normals[0], GL_STATIC_DRAW );
    glBindBufferARB( GL_ARRAY_BUFFER, 0 );
    
    if( glIsBufferARB( ibo ) )
        glDeleteBuffersARB( 1, &ibo );
    glGenBuffersARB( 1, &ibo );
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, ibo );
    glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( unsigned int ), &indices[0], GL_STATIC_DRAW );    
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void Mannequin::computeCenterOfGravity(){

    // Declarations.
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

void Mannequin::draw(){

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
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, Vector4D( MANNEQUIN_COLOR, 1 ).Array() );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, Vector4D( MANNEQUIN_COLOR, 1 ).Array() );
    // glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, Vector4D( MANNEQUIN_COLOR, 1 ).Array() );
    
    // Color.
    glColor3f( MANNEQUIN_COLOR );

    // VBO update and draw.
    glBindBufferARB( GL_ARRAY_BUFFER, vbo );
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, NULL );

    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, ibo );

    glBindBufferARB( GL_ARRAY_BUFFER, nbo );
    glEnableClientState( GL_NORMAL_ARRAY );
    glNormalPointer( GL_FLOAT, 12, NULL );
//    glNormalPointer( GL_FLOAT, offsetof( Mannequin, normals ), NULL );

    glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );
    glBindBufferARB( GL_ARRAY_BUFFER, 0 );
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, 0 );
    //*/

    /* Traditional draw. Requires "WithFaces".
    unsigned int i;
    glBegin( GL_TRIANGLES );
    for( i = 0; i < vertices.size(); i += 3 ){
        glVertex3f( vertices[i].x, vertices[i].y, vertices[i].z );
        glVertex3f( vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z );
        glVertex3f( vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z );
        glNormal3f( normals[i / 3].x, normals[i / 3].y, normals[i / 3].z );
    }
    glEnd();
    //*/

    glPopMatrix();
    glEnable( GL_COLOR_MATERIAL );
}

void Mannequin::tiltForward( float _rotation ){
    rotation.x -= _rotation;
}

void Mannequin::tiltSideways( float _rotation ){
    rotation.z -= _rotation;
}
