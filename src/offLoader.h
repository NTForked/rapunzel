/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                                 OFF Loader
/-------------------------------------------------------------------------------
*/

#ifndef OFF_LOADER_H
#define OFF_LOADER_H



#include "HeadersAndDefines.h"

enum offType{
    StraightForward,
    WithFaces
};

class offLoader : public Object3D{
public:
    unsigned int nOfVertices, nOfFaces, nOfEdges;
    std::vector<unsigned int> faces;
    std::vector<Vector3D> vertices, normals, finalVertices;
    offType type;

    // Constructor.
    offLoader( offType _type );

    void readOFF( const std::string filename );
};

#endif
