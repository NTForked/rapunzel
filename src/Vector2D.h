/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                               2D Vector Model
/-------------------------------------------------------------------------------
*/

#ifndef Vector2D_H
#define Vector2D_H



#include "HeadersAndDefines.h"

class Vector2D{

public:
    GLfloat x, y;

    // Constructors
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    Vector2D( GLfloat _x, GLfloat _y ){
        x = _x;
        y = _y;
    }
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    Vector2D(){
        x = 0;
        y = 0;
    }

    // Operator Overloads
    //> Unary operators.
    #ifdef __CUDA_ARCH__
    __device__
    #endif
    inline Vector2D operator-() const {
        return Vector2D( -x, -y );
    }

    //> Binary operators.
    //>> Vector-vector operators.
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    inline Vector2D operator+( const Vector2D& V2 ) const {
        return Vector2D( x + V2.x, y + V2.y );
    }
    #ifdef __CUDA_ARCH__
    __device__
    #endif
    inline Vector2D operator-( const Vector2D& V2 ) const {
        return Vector2D( x - V2.x, y - V2.y );
    }
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    inline Vector2D operator/( const Vector2D& V2 ) const {
        return Vector2D( x / V2.x, y / V2.y );
    }
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    inline Vector2D operator*( const Vector2D& V2 ) const {
        return Vector2D( x * V2.x, y * V2.y );
    }
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    inline bool operator==( const Vector2D& V2 ) const {
        return x == V2.x && y == V2.y;
    }
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    inline void operator+=( const Vector2D& V2 ){
        x += V2.x;
        y += V2.y;
    }
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    inline void operator-=( const Vector2D& V2 ){
        x -= V2.x;
        y -= V2.y;
    }
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    inline void operator*=( const Vector2D& V2 ){
        x *= V2.x;
        y *= V2.y;
    }
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    inline void operator/=( const Vector2D& V2 ){
        x /= V2.x;
        y /= V2.y;
    }

    //>> Vector-scalar operators.
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    inline Vector2D operator/( GLfloat s ) const {
        return Vector2D( x / s, y / s );
    }
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    inline Vector2D operator*( GLfloat s ) const {
        return Vector2D( x * s,  y * s );
    }
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    inline GLfloat operator[]( int i ){
        if ( i == 0 )
            return x;
        return y;
    }

    // Functions
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    GLfloat Length() const {
        return sqrtf( x * x + y * y );
    }
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    GLfloat Distance( const Vector2D &V1 ) const {
        return ( *this - V1 ).Length();
    }
    #ifdef __CUDA_ARCH__
    __device__ 
    #endif
    inline Vector2D normalize(){
        GLfloat fMag = x * x + y * y;
        if( fMag == 0 )
            return Vector2D( 0, 0 );

        GLfloat fMult = 1.0f / sqrtf( fMag );            
        x *= fMult;
        y *= fMult;
        return Vector2D( x, y );
    }

    static float *arr;

    float *Array(){
        arr[0] = x;
        arr[1] = y;
        return arr;
    }
};

#endif