/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                               3D Vector Model
/-------------------------------------------------------------------------------
*/

#ifndef Vector3D_H
#define Vector3D_H

#include "HeadersAndDefines.h"

class Vector3D {

public:
    GLfloat x, y, z;

// Constructors
#ifdef __CUDA_ARCH__
    __device__
#endif
    Vector3D(GLfloat _x, GLfloat _y, GLfloat _z) {
        x = _x;
        y = _y;
        z = _z;
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
    Vector3D() {
        x = 0;
        y = 0;
        z = 0;
    }

// Operator Overloads
//> Unary operators.
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector3D
        operator-() const {
        return Vector3D(-x, -y, -z);
    }

//> Binary operators.
//>> Vector-vector operators.
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector3D
        operator+(const Vector3D& V2) const {
        return Vector3D(x + V2.x, y + V2.y, z + V2.z);
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector3D
        operator-(const Vector3D& V2) const {
        return Vector3D(x - V2.x, y - V2.y, z - V2.z);
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector3D
        operator/(const Vector3D& V2) const {
        return Vector3D(x / V2.x, y / V2.y, z / V2.z);
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector3D
        operator*(const Vector3D& V2) const {
        return Vector3D(x * V2.x, y * V2.y, z * V2.z);
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline bool
        operator==(const Vector3D& V2) const {
        return x == V2.x && y == V2.y && z == V2.z;
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline void
        operator+=(const Vector3D& V2) {
        x += V2.x;
        y += V2.y;
        z += V2.z;
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline void
        operator-=(const Vector3D& V2) {
        x -= V2.x;
        y -= V2.y;
        z -= V2.z;
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline void
        operator*=(const Vector3D& V2) {
        x *= V2.x;
        y *= V2.y;
        z *= V2.z;
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline void
        operator/=(const Vector3D& V2) {
        x /= V2.x;
        y /= V2.y;
        z /= V2.z;
    }

//>> Vector-scalar operators.
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector3D
        operator/(GLfloat s) const {
        return Vector3D(x / s, y / s, z / s);
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector3D
        operator*(GLfloat s) const {
        return Vector3D(x * s, y * s, z * s);
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline GLfloat
        operator[](int i) {
        if (i == 0)
            return x;
        if (i == 1)
            return y;
        return z;
    }

// Functions
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector3D
        CrossProduct(const Vector3D& V2) const {
        return Vector3D(y * V2.z - z * V2.y, z * V2.x - x * V2.z,
                        x * V2.y - y * V2.x);
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        GLfloat
        Length() const {
        return sqrtf(x * x + y * y + z * z);
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        GLfloat
        Distance(const Vector3D& V1) const {
        return (*this - V1).Length();
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector3D
        normalize() {
        GLfloat fMag = x * x + y * y + z * z;
        if (fMag == 0)
            return Vector3D(0, 0, 0);

        GLfloat fMult = 1.0f / sqrtf(fMag);
        x *= fMult;
        y *= fMult;
        z *= fMult;
        return Vector3D(x, y, z);
    }

    static float* arr;

    float* Array() {
        arr[0] = x;
        arr[1] = y;
        arr[2] = z;
        return arr;
    }
};

#endif
