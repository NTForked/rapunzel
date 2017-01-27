/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                               Viewport Camera
/                                          ^ y
/                                         /
/                                        /--
/                                       /  /
/                                      +------> x
/                                      |
/                                      |
/                                      |
/                                      V z
/-------------------------------------------------------------------------------
*/

#include "Camera.h"

// Constructor with standard OpenGL values.
Camera::Camera() {
    position = Vector3D(O);
    forward = Vector3D(OZ).normalize();
    right = Vector3D(OX).normalize();
    up = Vector3D(OY).normalize();
}

// Translations.
void
Camera::setPosition(Vector3D _position) {
    position = _position;
}

void
Camera::moveForward(GLfloat distance) {
    position += forward * distance;
}

void
Camera::moveRight(GLfloat distance) {
    position += right * distance;
}

void
Camera::moveUpward(GLfloat distance) {
    position += up * distance;
}

void
Camera::moveForwardGrounded(GLfloat distance) {
    float y = forward.y;
    forward.y = 0;
    moveForward(distance);
    forward.y = y;
}

void
Camera::moveRightGrounded(GLfloat distance) {
    float y = right.y;
    right.y = 0;
    moveRight(distance);
    right.y = y;
}

void
Camera::checkAngle(float &angle) {
    if (angle > 2 * (float)M_PI)
        angle -= 2 * (float)M_PI;
    else if (angle < 0)  //-2 * (float)M_PI )
        angle += 2 * (float)M_PI;
}

void
Camera::rotateFPS_OX(float angle) {

    // Declarations.
    Vector3D projectedForward, projectedUp;
    GLfloat projectedForwardLength;

    // Sanity check.
    checkAngle(angle);

    projectedForward = forward;
    projectedUp = up;
    projectedForward.x = 0;
    projectedUp.x = 0;
    projectedForwardLength = projectedForward.Length();
    projectedForward.normalize();
    projectedUp.normalize();

    projectedForward = projectedForward * cos(angle) -
                       projectedUp * sin(angle);  // from OZ towrads OX
    projectedForward = projectedForward.normalize() * projectedForwardLength;
    projectedForward.x = forward.x;
    forward = projectedForward.normalize();
    up = (projectedUp * cos(angle) - projectedForward * sin(angle)).normalize();
    right = forward.CrossProduct(up).normalize();
}

void
Camera::rotateFPS_OY(float angle) {

    // Declarations.
    Vector3D projectedForward, projectedRight;
    GLfloat projectedForwardLength;

    // Sanity checks.
    checkAngle(angle);

    projectedForward = forward;
    projectedRight = right;
    projectedForward.y = 0;
    projectedRight.y = 0;
    projectedForwardLength = projectedForward.Length();
    projectedForward.normalize();
    projectedRight.normalize();

    projectedForward = projectedForward * cos(angle) +
                       projectedRight * sin(angle);  // from OZ towrads OX
    projectedForward = projectedForward.normalize() * projectedForwardLength;
    projectedForward.y = forward.y;
    forward = projectedForward.normalize();
    right = (projectedRight * cos(angle) - projectedForward * sin(angle))
                .normalize();
    up = right.CrossProduct(forward).normalize();
}

void
Camera::rotateTPS_OX(float angle, float distance) {
    moveForward(distance);
    rotateFPS_OX(angle);
    moveForward(-distance);
}

void
Camera::rotateTPS_OY(float angle, float distance) {
    moveForward(distance);
    rotateFPS_OY(angle);
    moveForward(-distance);
}

void
Camera::lookAt(Vector3D point) {
    forward = (point - position).normalize();
    rotateFPS_OX(0);
    rotateFPS_OY(0);
}

// Placing observer in scene.
void
Camera::render() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Sanity check.
    if (up.y < 0)
        up.y = -up.y;

    Vector3D viewPoint = position + forward;  // Point at which camera points.

    gluLookAt(position.x, position.y, position.z, viewPoint.x, viewPoint.y,
              viewPoint.z, up.x, up.y, up.z);
}
