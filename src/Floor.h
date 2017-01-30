/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                                Floor Object
/-------------------------------------------------------------------------------
*/

#ifndef FLOOR_H
#define FLOOR_H

#include "HeadersAndDefines.h"

class Floor : public Object3D {
public:
    // Fields.
    Vector3D dimensions;
    GLuint surfaceTexture, vbo, nbo, ibo;
    bmpLoader bmp;
    std::vector<Vector3D> vertices, normals;
    std::vector<Vector2D> textures;
    std::vector<unsigned int> indices;

    // Constructor.
    Floor();

    // Draw.
    GLuint loadBMP(const std::string textureFile);
    void draw();
};

#endif
