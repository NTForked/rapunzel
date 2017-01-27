/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                               General Object
/-------------------------------------------------------------------------------
*/

#include "Object3D.h"

// Static variables.
Vector3D Object3D::selectedColor = Vector3D(RED_COLOR);
Vector3D Object3D::colorIncrement = Vector3D(
    0.04f,
    0.04f,
    0.04f);  // Value with which SelectedColor is incremented / decremented.

// Basic constructor.
Object3D::Object3D() {
    defaultSettings();
}

// Constructor with default settings.
void
Object3D::defaultSettings() {
    translation = Vector3D(0, 0, 0);
    rotation = Vector3D(0, 0, 0);
    scale = Vector3D(1, 1, 1);
    normal = Vector3D(0, 1, 0);

    wireframe = false;
    visible = true;
    selected = false;

    color = Vector3D(BLUE_COLOR);
    selectedColor = Vector3D(RED_COLOR);
    colorIncrement = Vector3D(COLOR_INCREMENT);
}

Vector3D
Object3D::computeNormal(Vector3D vertex1, Vector3D vertex2, Vector3D vertex3) {
    Vector3D A, B, normal;

    A.x = vertex1.x - vertex2.x;
    A.y = vertex1.y - vertex2.y;
    A.z = vertex1.z - vertex2.z;

    B.x = vertex2.x - vertex3.x;
    B.y = vertex2.y - vertex3.y;
    B.z = vertex2.z - vertex3.z;

    normal.x = A.y * B.z - A.z * B.y;
    normal.y = A.z * B.x - A.x * B.z;
    normal.z = A.x * B.y - A.y * B.x;

    return normal;
}

void
Object3D::draw() {

    if (!visible)
        return;

    glPushMatrix();

    // Translation.
    glTranslatef(translation.x, translation.y, translation.z);

    // Rotation.
    glRotatef(rotation.x, 1.0, 0.0, 0.0);
    glRotatef(rotation.y, 0.0, 1.0, 0.0);
    glRotatef(rotation.z, 0.0, 0.0, 1.0);

    // Scale.
    glScalef(scale.x, scale.y, scale.z);

    // Material settings.
    if (!selected) {
        glColor3f(color.x, color.y, color.z);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                     Vector4D(color.x, color.y, color.z, 1).Array());
    } else {
        glColor3f(selectedColor.x, selectedColor.y, selectedColor.z);
        glMaterialfv(
            GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
            Vector4D(selectedColor.x, selectedColor.y, selectedColor.z, 1)
                .Array());
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                 Vector4D(0.1f, 0.1f, 0.1f, 1).Array());

    glPopMatrix();
}

void
Object3D::select() {
    selected = true;
}

void
Object3D::deselect() {
    selected = false;
}
