/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                               4D Vector Model
/-------------------------------------------------------------------------------
*/

#ifndef Vector4D_H
#define Vector4D_H

#include "HeadersAndDefines.h"

class Vector4D {

public:
    GLfloat x, y, z, a;

// Constructors
#ifdef __CUDA_ARCH__
    __device__
#endif
    Vector4D(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _a) {
        x = _x;
        y = _y;
        z = _z;
        a = _a;
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
    Vector4D() {
        x = 0;
        y = 0;
        z = 0;
        a = 0;
    }

// Operator Overloads
//> Unary operators.
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector4D
        operator-() const {
        return Vector4D(-x, -y, -z, -a);
    }

//> Binary operators.
//>> Vector-vector operators.
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector4D
        operator+(const Vector4D& V2) const {
        return Vector4D(x + V2.x, y + V2.y, z + V2.z, a + V2.a);
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector4D
        operator-(const Vector4D& V2) const {
        return Vector4D(x - V2.x, y - V2.y, z - V2.z, a - V2.a);
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector4D
        operator*(const Vector4D& V2) const {
        return Vector4D(x * V2.x, y * V2.y, z * V2.z, a * V2.a);
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector4D
        operator/(const Vector4D& V2) const {
        return Vector4D(x / V2.x, y / V2.y, z / V2.z, a / V2.a);
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline bool
        operator==(const Vector4D& V2) const {
        return x == V2.x && y == V2.y && z == V2.z && a == V2.a;
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline void
        operator+=(const Vector4D& V2) {
        x += V2.x;
        y += V2.y;
        z += V2.z;
        a += V2.a;
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline void
        operator-=(const Vector4D& V2) {
        x -= V2.x;
        y -= V2.y;
        z -= V2.z;
        a -= V2.a;
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline void
        operator*=(const Vector4D& V2) {
        x *= V2.x;
        y *= V2.y;
        z *= V2.z;
        a *= V2.a;
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline void
        operator/=(const Vector4D& V2) {
        x /= V2.x;
        y /= V2.y;
        z /= V2.z;
        a /= V2.a;
    }

//>> Vector-scalar operators.
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector4D
        operator*(GLfloat s) const {
        return Vector4D(x / s, y / s, z / s, a / s);
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector4D
        operator/(GLfloat s) const {
        return Vector4D(x / s, y / s, z / s, a / s);
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
        if (i == 2)
            return z;
        return a;
    }

// Functions.
#ifdef __CUDA_ARCH__
    __device__
#endif
        GLfloat
        Length() const {
        return sqrtf(x * x + y * y + z * z + a * a);
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        GLfloat
        Distance(const Vector4D& V1) const {
        return (*this - V1).Length();
    }
#ifdef __CUDA_ARCH__
    __device__
#endif
        inline Vector4D
        normalize() {
        GLfloat fMag = (x * x + y * y + z * z + a * a);
        if (fMag == 0)
            return Vector4D(0, 0, 0, 0);

        GLfloat fMult = 1.0f / sqrtf(fMag);
        x *= fMult;
        y *= fMult;
        z *= fMult;
        a *= fMult;
        return Vector4D(x, y, z, a);
    }

    static float* arr;

    float* Array() {
        arr[0] = x;
        arr[1] = y;
        arr[2] = z;
        arr[3] = a;
        return arr;
    }
};

#endif
