/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                                 OFF Loader
/-------------------------------------------------------------------------------
*/

#include "offLoader.h"

offLoader::offLoader(offType _type) : Object3D() {
    nOfVertices = 0;
    nOfFaces = 0;
    nOfEdges = 0;
    type = _type;
    faces = std::vector<unsigned int>();
    vertices = std::vector<Vector3D>();
    normals = std::vector<Vector3D>();
    finalVertices = std::vector<Vector3D>();
}

void
offLoader::readOFF(const std::string filename) {

    // Declarations.
    std::ifstream input;
    unsigned int i, j, value;
    float x, y, z;

    input.open(filename);
    if (!input.is_open())
        errorHandler(ERROR_CODE1,
                     std::string("ERROR 1: Unable to read from file \"" +
                                 filename + "\"."),
                     "TODO: GetLastErrorAsString()");

    std::string line;
    input >> line;
    if (line != "OFF")
        errorHandler(ERROR_CODE2, std::string("ERROR 2: Not and OFF file."),
                     "TODO: GetLastErrorAsString()");

    input >> nOfVertices >> nOfFaces >> nOfEdges;
    for (i = 0; i < nOfVertices; i += 1) {
        input >> x >> y >> z;
        vertices.push_back(Vector3D(x, y, z));
    }
    for (i = 0; i < nOfFaces; i += 1) {
        input >> nOfEdges;
        if (nOfEdges > 3)
            errorHandler(
                ERROR_CODE3,
                std::string("ERROR 3: OFF has faces with more than 3 edges."),
                "TODO: GetLastErrorAsString()");

        for (j = 0; j < nOfEdges; j += 1) {
            input >> value;
            faces.push_back(value);
        }
    }
    if (type == StraightForward) {
        Vector3D normal;
        normals.resize(vertices.size());
        for (i = 0; i < nOfFaces; i += 1) {
            normal = computeNormal(vertices[faces[3 * i]],
                                   vertices[faces[3 * i + 1]],
                                   vertices[faces[3 * i + 2]]);
            normals[faces[3 * i]] = normal;
            normals[faces[3 * i + 1]] = normal;
            normals[faces[3 * i + 2]] = normal;
        }
        finalVertices = vertices;
    } else {
        for (i = 0; i < nOfFaces; i += 1) {
            finalVertices.push_back(vertices[faces[3 * i]]);
            finalVertices.push_back(vertices[faces[3 * i + 1]]);
            finalVertices.push_back(vertices[faces[3 * i + 2]]);
            normals.push_back(computeNormal(finalVertices[3 * i],
                                            finalVertices[3 * i + 1],
                                            finalVertices[3 * i + 2]));
        }
    }
}
