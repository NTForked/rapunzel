/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                                 Light Model
/-------------------------------------------------------------------------------
*/

#ifndef LIGHT_H
#define LIGHT_H

#include "HeadersAndDefines.h"

enum IlluminationType {
    Ideal,  // Omnidirectional light.
    Spot    // Directional light.
};

class Light : public Object3D {
public:
    // Fields.
    static int baseId;
    int id;  // Starts with 0 and is used for GL_LIGHT0 + id.
    IlluminationType LightType;

    // Constructor.
    Light();

    // Methods.
    void render(Vector3D position,
                Vector3D direction);  // Places light in scene and activates it.
    void disable();                   // Deactivate light.
    void setLightType(IlluminationType LightType);
    GLboolean isEnabled();
};

#endif
