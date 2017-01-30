/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                                 Light Model
/-------------------------------------------------------------------------------
*/

#include "Light.h"

int Light::baseId = 0;

// Constructor with type parameter.
Light::Light() : Object3D() {
    // Default values.
    baseId = 0;
    id = baseId;  // id is unique.
    baseId += 1;
    LightType = Ideal;  // By default, light is omnidrectional.
}

void
Light::setLightType(IlluminationType _LightType) {
    LightType = _LightType;
}

void
Light::render(Vector3D position,
              Vector3D /* direction */) {  // Placing light in scene.

    // Colors.
    glLightfv(GL_LIGHT0 + id, GL_AMBIENT, Vector4D(WHITE_COLOR, 1).Array());
    glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, Vector4D(WHITE_COLOR, 1).Array());
    glLightfv(GL_LIGHT0 + id, GL_SPECULAR, Vector4D(WHITE_COLOR, 1).Array());

    // Positioning and directions.
    glLightfv(GL_LIGHT0 + id, GL_POSITION,
              Vector4D(position.x, position.y, position.z, 0).Array());
    //> Ideal.
    // glLightfv( GL_LIGHT0 + id , GL_SPOT_DIRECTION, Vector3D( 0, 0, 0
    // ).Array() );
    //> Spot.
    // glLightfv( GL_LIGHT0 + id , GL_SPOT_DIRECTION,
    // direction.normalize().Array() );
    // glLightf( GL_LIGHT0 + id , GL_SPOT_CUTOFF, 90.0f );
    // glLightf( GL_LIGHT0 + id, GL_SPOT_EXPONENT, 2 );

    /* Standard attenuations.
    glLightf( GL_LIGHT0 + id, GL_CONSTANT_ATTENUATION, 1.0f );
    glLightf( GL_LIGHT0 + id, GL_LINEAR_ATTENUATION, 0.0f );
    glLightf( GL_LIGHT0 + id, GL_QUADRATIC_ATTENUATION, 0.0f );
    //*/
    glEnable(GL_LIGHT0 + id);  // Activating light.
}

// Deactivating light.
void
Light::disable() {
    glDisable(GL_LIGHT0 + id);
}

GLboolean
Light::isEnabled() {
    return glIsEnabled(GL_LIGHT0 + id);
}
