/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                                 Hair Object
/-------------------------------------------------------------------------------
*/

#ifndef HAIR_H
#define HAIR_H

#include "HeadersAndDefines.h"
#include "offLoader.h"
#include <cmath>

enum HairType { HumanHair, AnimalFur };

enum HairStyle { StraightUp, Locks, OverHead, Custom, None };

// windDirection.
const Vector3D X = Vector3D(-1.0f, 0.0f, 0.0f);
const Vector3D MinusX = Vector3D(1.0f, 0.0f, 0.0f);
const Vector3D Z = Vector3D(0.0f, 0.0f, -1.0f);
const Vector3D MinusZ = Vector3D(0.0f, 0.0f, 1.0f);

class Hair : public Object3D {
public:
    // Fields.
    HairType type;
    HairStyle style;
    offLoader *off;
    unsigned int length;
    Vector3D centerOfGravity, downsizedCenterOfGravity, upsizedCenterOfGravity,
        windDirection;
    std::vector<Vector3D> normals;
    std::vector<Vector3D> vertices;
    std::vector<Vector3D> previousVertices;
    std::vector<unsigned int> indices;
    std::vector<float> distancesUpToCenter;
    float greatestY, delta, timeSinceLastSimulation;
    GLuint vbo, ibo;
    bool wind;
    unsigned int gravity;
    std::string runsOn;

    // Constructor.
    Hair(HairType _type,
         HairStyle _style,
         unsigned int _length,
         std::string _runsOn,
         std::string HAIR_OFF);

    float min4f(float a, float b, float c, float d);
    float max4f(float a, float b, float c, float d);
    void computeParameters();
    void draw();
    void draw(Vector3D color);
    Vector3D getNextVertex(Vector3D oldVertex,
                           Vector3D centerOfGravity,
                           float topHeight,
                           unsigned int iteration);
    void computeCenterOfGravity();
    void computeCustomDraw();
    void tiltForward(float _rotation);
    void tiltSideways(float _rotation);

    // Simulation.
    float collisionOffset(Vector3D vertex, Vector3D referenceVertex);
    float collisionOffsetAfterFall(Vector3D vertex,
                                   Vector3D referenceVertex,
                                   float fallSpeed);
    float collisionOffsetAfterWind(Vector3D vertex,
                                   Vector3D referenceVertex,
                                   float windSpeed);
    void restoreFromCollision(Vector3D &vertex,
                              Vector3D previousVertex,
                              Vector3D referenceVertex);
    bool stuck(Vector3D vertex, Vector3D previousVertex);
    void fall(Vector3D &vertex,
              Vector3D copy,
              Vector3D referenceVertex,
              float fallSpeed);
    void negativeGravity(Vector3D &vertex,
                         Vector3D copy,
                         Vector3D referenceVertex,
                         float fallSpeed);
    void blowWind(Vector3D &vertex,
                  Vector3D previousVertex,
                  Vector3D previousPreviousVertex,
                  float windSpeed,
                  float t,
                  float t0);  // No, not that kind of wind, smarty pants!
    void simulateAnimation();
};

#endif
