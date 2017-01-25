/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                               General Object
/-------------------------------------------------------------------------------
*/

#ifndef OBJECT3D_H
#define OBJECT3D_H



#include "HeadersAndDefines.h"

class Object3D{

public: // start with small letter
    Vector3D translation;
    Vector3D rotation;
    Vector3D scale;
    Vector3D color;
    Vector3D normal;

    bool selected;

    static Vector3D selectedColor;
    static Vector3D colorIncrement;
    bool wireframe;
    bool visible;

    // Constructors.
    Object3D();
    Object3D( Vector3D position );
    Object3D( Vector3D position, Vector3D rotation, Vector3D scale );

    void virtual draw();

    Vector3D computeNormal( Vector3D vertex1, Vector3D vertex2, Vector3D vertex3 );

    // Setters for all non-public variables.
    void select();
    void deselect();

     // Set default settings.
    void defaultSettings();
    
};

#endif
