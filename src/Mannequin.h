/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                               Mannequin Object
/-------------------------------------------------------------------------------
*/

#ifndef MANNEQUIN_H
#define MANNEQUIN_H



#include "HeadersAndDefines.h"
#include "offLoader.h"

enum MannequinType{
    Human,
    Animal
};

class Mannequin : public Object3D{
public:
    // Fields.
    MannequinType type;
    float smallestX, greatestX, smallestY, greatestY, smallestZ, greatestZ;
    Vector3D centerOfGravity;
    GLuint vbo, nbo, ibo;
    offLoader *off;

    // OFF specific.
    std::vector<Vector3D> normals;
    std::vector<Vector3D> vertices;
    std::vector<unsigned int> indices;

    // Constructor.
    Mannequin( MannequinType type );

    void Mannequin::computeCenterOfGravity();
    void draw();
    void tiltForward( float _rotation );
    void tiltSideways( float _rotation );
};

#endif
