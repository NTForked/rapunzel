/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                                Floor Object
/-------------------------------------------------------------------------------
*/

#include "Floor.h"

GLuint
Floor::loadBMP(const std::string textureFile) {

    // Declarations;
    std::string data;
    GLuint tex;

    data = bmp.loadBMP(textureFile);
    glGenBuffersARB(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.width, bmp.height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data.c_str());
    glBindTexture(GL_TEXTURE_2D, 0);
    return tex;
}

// Constructor.
Floor::Floor() : Object3D() {

    // Declarations.
    GLfloat x, y, z;
    unsigned int i;

    // Default values.
    dimensions = Vector3D(1024.0f, 10.0f, 1024.0f);
    x = dimensions.x / 2.0f;
    y = dimensions.y / 2.0f;
    z = dimensions.z / 2.0f;

    // In case I decide I want VBO for this.
    vertices.resize(8);
    vertices[0] = Vector3D(-x, -y, -z);
    vertices[1] = Vector3D(-x, -y, z);
    vertices[2] = Vector3D(-x, y, -z);
    vertices[3] = Vector3D(-x, y, z);
    vertices[4] = Vector3D(x, -y, -z);
    vertices[5] = Vector3D(x, -y, z);
    vertices[6] = Vector3D(x, y, -z);
    vertices[7] = Vector3D(x, y, z);

    textures.resize(4);
    textures[0] = Vector2D(1.0f, 0.0f);
    textures[1] = Vector2D(0.0f, 0.0f);
    textures[2] = Vector2D(0.0f, 1.0f);
    textures[3] = Vector2D(1.0f, 1.0f);

    normals.resize(8);
    normals[0] = Vector3D(-1.0f, -1.0f, -1.0f);
    normals[1] = Vector3D(-1.0f, -1.0f, 1.0f);
    normals[2] = Vector3D(-1.0f, 1.0f, -1.0f);
    normals[3] = Vector3D(-1.0f, 1.0f, 1.0f);
    normals[4] = Vector3D(1.0f, -1.0f, -1.0f);
    normals[5] = Vector3D(1.0f, -1.0f, 1.0f);
    normals[6] = Vector3D(1.0f, 1.0f, -1.0f);
    normals[7] = Vector3D(1.0f, 1.0f, 1.0f);
    for (i = 0; i < normals.size(); i += 1)
        normals[i] = normals[i].normalize();

    indices.resize(24);
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 3;
    indices[3] = 2;
    indices[4] = 2;
    indices[5] = 3;
    indices[6] = 7;
    indices[7] = 6;
    indices[8] = 3;
    indices[9] = 1;
    indices[10] = 5;
    indices[11] = 7;
    indices[12] = 0;
    indices[13] = 2;
    indices[14] = 6;
    indices[15] = 4;
    indices[16] = 1;
    indices[17] = 0;
    indices[18] = 4;
    indices[19] = 5;
    indices[20] = 5;
    indices[21] = 4;
    indices[22] = 6;
    indices[23] = 7;
    //*/
    // VBO generation.
    glGenBuffersARB(1, &vbo);
    glBindBufferARB(GL_ARRAY_BUFFER, vbo);
    glBufferDataARB(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3D),
                    &vertices[0], GL_STATIC_DRAW);
    glBindBufferARB(GL_ARRAY_BUFFER, 0);

    glGenBuffersARB(1, &nbo);
    glBindBufferARB(GL_ARRAY_BUFFER, nbo);
    glBufferDataARB(GL_ARRAY_BUFFER, normals.size() * sizeof(Vector3D),
                    &normals[0], GL_STATIC_DRAW);
    glBindBufferARB(GL_ARRAY_BUFFER, 0);

    glGenBuffersARB(1, &ibo);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER,
                    indices.size() * sizeof(unsigned int), &indices[0],
                    GL_STATIC_DRAW);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

    surfaceTexture = loadBMP(SURFACE_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, surfaceTexture);
    glBindBufferARB(GL_ARRAY_BUFFER, surfaceTexture);
    glBufferDataARB(GL_ARRAY_BUFFER, textures.size() * sizeof(Vector2D),
                    &textures[0], GL_STATIC_DRAW);
    glBindBufferARB(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Overwriting by polimorphism.
void
Floor::draw() {

    if (!visible)
        return;

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Rotation.
    glRotatef(rotation.x, 1.0, 0.0, 0.0);
    glRotatef(rotation.y, 0.0, 1.0, 0.0);
    glRotatef(rotation.z, 0.0, 0.0, 1.0);

    // Translation.
    glTranslatef(translation.x, translation.y, translation.z);

    // Scale.
    glScalef(scale.x, scale.y, scale.z);

    // Material settings.
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
                 Vector4D(MANNEQUIN_COLOR, 1).Array());
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
                 Vector4D(MANNEQUIN_COLOR, 1).Array());
    // glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, Vector4D( MANNEQUIN_COLOR,
    // 1 ).Array() );

    // Color.
    glColor3f(MANNEQUIN_COLOR);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // VBO update and draw.
    glBindBufferARB(GL_ARRAY_BUFFER, vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, NULL);

    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBindBufferARB(GL_ARRAY_BUFFER, nbo);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 12, NULL);
    //    glNormalPointer( GL_FLOAT, offsetof( Mannequin, normals ), NULL );

    glBindBufferARB(GL_ARRAY_BUFFER, surfaceTexture);
    glBindTexture(GL_TEXTURE_2D, surfaceTexture);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, NULL);

    glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBufferARB(GL_ARRAY_BUFFER, 0);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
    //*/

    // Traditional Draw.
    glBindTexture(GL_TEXTURE_2D, surfaceTexture);
    glBegin(GL_QUADS);
    unsigned int i;
    for (i = 0; i < 6; i += 1) {
        glNormal3f(normals[indices[4 * i]].x, normals[indices[4 * i]].y,
                   normals[indices[4 * i]].z);
        glTexCoord2f(textures[0].x, textures[0].y);
        glVertex3f(vertices[indices[4 * i]].x, vertices[indices[4 * i]].y,
                   vertices[indices[4 * i]].z);
        glTexCoord2f(textures[1].x, textures[1].y);
        glVertex3f(vertices[indices[4 * i + 1]].x,
                   vertices[indices[4 * i + 1]].y,
                   vertices[indices[4 * i + 1]].z);
        glTexCoord2f(textures[2].x, textures[2].y);
        glVertex3f(vertices[indices[4 * i + 2]].x,
                   vertices[indices[4 * i + 2]].y,
                   vertices[indices[4 * i + 2]].z);
        glTexCoord2f(textures[3].x, textures[3].y);
        glVertex3f(vertices[indices[4 * i + 3]].x,
                   vertices[indices[4 * i + 3]].y,
                   vertices[indices[4 * i + 3]].z);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    //*/
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
