/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                             Headers And Defines
/-------------------------------------------------------------------------------
*/

#ifndef HEADERS_AND_DEFINES_H
#define HEADERS_AND_DEFINES_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdio>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <GL/gl.h>
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Object3D.h"
#include "bmpLoader.h"
// #include <boost/numeric/odeint.hpp>
/*
#include "shaderLoader.h"
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"
*/
// CUDA header includes
#include <cuda.h>
#include <cudaGL.h>
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#include <device_launch_parameters.h>

// General hair parameters.
#ifdef __CUDA_ARCH__
__device__
#endif
static unsigned int    HAIR_LENGTH                            = 2;        // Number of hairs over hair base.
#ifdef __CUDA_ARCH__
__device__
#endif
const float            HAIR_STRAND_LENGTH                    = 0.10f;
#ifdef __CUDA_ARCH__
__device__
#endif
const float            HAIR_ELASTICITY_FACTOR                = 0.00f;
// Hair collision parameters.
const float            COLLISION_PADDING_OFFSET            = 0.10f;    // Should be between 0.00f and 0.10f.
#ifdef __CUDA_ARCH__
__device__
#endif
const float            RESTORATION_OFFSET                    = 0.00f;    // Extra distance for hair restoration.
// Falling hair parameters.
const float            PROXIMITY_OFFSET                    = 0.70f;    // Deprecated.
const float            PROXIMITY_FACTOR                    = 1.00f;    // Deprecated.
const float            Y_DOWNSIZE_OFFSET                    = HAIR_LENGTH * HAIR_STRAND_LENGTH + 1.00f;
const float            FALLING_ELASTIC_RESTORATION_FACTOR    = 0.01f;    // Deprecated.
#ifdef __CUDA_ARCH__
__device__
#endif
const float            FALL_SPEED                            = 0.01f;
// Hair wind parameters.
const float            WIND_SPEED                            = 0.06f;
#ifdef __CUDA_ARCH__
__device__
#endif
const float            HAIR_WAVE_HEIGHT                    = 0.6f;        // Deprecated.
#ifdef __CUDA_ARCH__
__device__
#endif
const float            HAIR_WAVE_LENGTH                    = 3.0f;
#ifdef __CUDA_ARCH__
__device__
#endif
const float            DELTA_INCREMENT                        = 0.3f;        // Deprecated.
// Hair gravity parameters.
#ifdef __CUDA_ARCH__
__device__
#endif
const unsigned int    GRAVITY_OFF                            = 0;
#ifdef __CUDA_ARCH__
__device__
#endif
const unsigned int    NORMAL_GRAVITY                        = 1;
#ifdef __CUDA_ARCH__
__device__
#endif
const unsigned int    NEGATIVE_GRAVITY                    = 2;
#ifdef __CUDA_ARCH__
__device__
#endif
const unsigned int    N_GRAVITY_TYPES                        = 3;
// Changeable definitions.
#ifdef __CUDA_ARCH__
__device__
#endif
const float            TILT_SPEED                            = 0.50f;
#ifdef __CUDA_ARCH__
__device__
#endif
const unsigned int    TILT_FORWARD                        = 0;
#ifdef __CUDA_ARCH__
__device__
#endif
const unsigned int    TILT_BACKWARDS                        = 1;
#ifdef __CUDA_ARCH__
__device__
#endif
const unsigned int    TILT_LEFT                            = 2;
#ifdef __CUDA_ARCH__
__device__
#endif
const unsigned int    TILT_RIGHT                            = 3;
const unsigned int    WINDOW_WIDTH                        = 800;
const unsigned int    WINDOW_HEIGHT                        = 600;
const unsigned int    WINDOW_STARTING_X                    = 200;
const unsigned int    WINDOW_STARTING_Y                    = 100;
const unsigned int    MENU_BLACK                            = 0;
#define                BLACK_HAIR                            0.0f, 0.0f, 0.0f
const unsigned int    MENU_BROWN                            = 1;
#define                BROWN_HAIR                            0.58f, 0.29f, 0.0f
const unsigned int    MENU_DARK_BROWN                        = 2;
#define                DARK_BROWN_HAIR                        0.39f, 0.26f, 0.13f
const unsigned int    MENU_AMBER                            = 3;
#define                AMBER_HAIR                            1.0f, 0.75f, 0.0f
const unsigned int    MENU_BLONDE                            = 4;
#define                BLONDE_HAIR                            0.75f, 0.75f, 0.50f
const unsigned int    MENU_RED                            = 5;
#define                RED_HAIR                            0.40f, 0.0f, 0.0f
const unsigned int    MENU_AUBURN                            = 6;
#define                AUBURN_HAIR                            0.65f, 0.16f, 0.16f
const double        Z_NEAR                                = 0.001;
const double        Z_FAR                                = 1000.0;
const double        FIELD_OF_VIEW_ANGLE                    = 60.0;
const float            SPEED                                = 5.00f;
const float            ROTATION_SPEED                        = 0.01f;
const float            MAX_SPEED                            = 0.01f;
const float            INF_DIM                                = 10000.00f;
const float            TOP_CAMERA_THRESHOLD                = 0.80f;
const float            BOTTOM_CAMERA_THRESHOLD                = -0.80f;
const float            HAIR_HEIGHT                            = 50.0f;
const float            CAMERA_ROTATION_SPEED                = (float)M_PI / 32.0f;
const unsigned int    MAX_COUNTERS                        = 10;
const unsigned int    FRAME_TIME                            = 0;
const unsigned int    CPU_PHYSICS_TIME                    = 1;
const unsigned int    GPU_PHYSICS_TIME                    = 2;
const unsigned int    MEASUREMENT_OVERHEAD_TIME            = 3;
const unsigned int    INTER_SIMULATION_TIME                = 4;
//> Resource indexes.
const unsigned int    HOST                                = 0;
const unsigned int    DEVICE                                = 1;
//> Error codes.
const int            ERROR_CODE_OFFSET                    = 100;
const int            ERROR_CODE1                            = EXIT_FAILURE + ERROR_CODE_OFFSET;
const int            ERROR_CODE2                            = EXIT_FAILURE + ERROR_CODE_OFFSET + 1;
const int            ERROR_CODE3                            = EXIT_FAILURE + ERROR_CODE_OFFSET + 2;
const int            ERROR_CODE4                            = EXIT_FAILURE + ERROR_CODE_OFFSET + 3;
const int            ERROR_CODE5                            = EXIT_FAILURE + ERROR_CODE_OFFSET + 4;
const int            ERROR_CODE6                            = EXIT_FAILURE + ERROR_CODE_OFFSET + 5;
const int            ERROR_CODE7                            = EXIT_FAILURE + ERROR_CODE_OFFSET + 6;
//---------------------------------------------------------------------------------------------
const int            ERROR_CODE8                            = EXIT_FAILURE + ERROR_CODE_OFFSET + 7;
const int            ERROR_CODE9                            = EXIT_FAILURE + ERROR_CODE_OFFSET + 8;
//> Colors.
#define                HAIR_COLOR                            0.20f, 0.00f, 0.00f
#define                LIPS_COLOR                            0.50f, 0.00f, 0.00f
#define                EYE_COLOR                            0.00f, 0.00f, 1.00f
#define                MANNEQUIN_COLOR                        0.82f, 0.71f, 0.53f
#define                BLUE_GLASS_COLOR                    0.20f, 0.71f, 0.93f
#define                LIGHT_BLUE_COLOR                    0.50f, 0.70f, 1.00f
#define                DARK_BLUE_COLOR                        0.00f, 0.30f, 0.60f
#define                WHITE_COLOR                            1.00f, 1.00f, 1.00f
#define             BLACK_COLOR                            0.00f, 0.00f, 0.00f
#define             GRAY_COLOR                            0.15f, 0.15f, 0.15f
#define             GREEN_COLOR                            0.27f, 0.73f, 0.43f
#define                RED_COLOR                    1.00f, 0.20f, 0.20f
#define             ORANGE_COLOR                0.99f, 0.51f, 0.20f
#define             BLUE_COLOR                    0.00f, 0.59f, 0.91f
#define             YELLOW_COLOR                1.00f, 1.00f, 0.00f
#define             COLOR_INCREMENT                0.01f, 0.01f, 0.01f
const std::string    BLUE                        = "\033[1;34m";
const std::string    GREEN                        = "\033[1;32m";
const std::string    RED                            = "\033[1;31m";
const std::string    STOP                        = "\033[0m";
const std::string    SURFACE_TEXTURE                = "Textures/surface.bmp";
const std::string    MANNEQUIN_OFF                = "OFF/mannequin.off";
const std::string    ANIMAL_OFF                    = "OFF/horse.off";
const std::string    SCALP_OFF                    = "OFF/scalp.off";
const std::string    SKIN_OFF                    = "OFF/skin.off";
const std::string    HAIR_OFF1                    = "OFF/humanHair1k.off";
const std::string    HAIR_OFF2                    = "OFF/humanHair2k.off";
const std::string    HAIR_OFF3                    = "OFF/humanHair3k.off";
const std::string    HAIR_OFF4                    = "OFF/humanHair4k.off";
const std::string    HAIR_OFF5                    = "OFF/humanHair5k.off";
const std::string    HAIR_OFF6                    = "OFF/humanHair6k.off";
const std::string    HAIR_OFF7                    = "OFF/humanHair7k.off";
const std::string    HAIR_OFF8                    = "OFF/humanHair8k.off";
const std::string    HAIR_OFF9                    = "OFF/humanHair9k.off";
const std::string    HAIR_OFF10                    = "OFF/humanHair10k.off";
const std::string    HAIR_OFF11                    = "OFF/humanHair11k.off";
const std::string    HAIR_OFF12                    = "OFF/humanHair12k.off";
const std::string    FUR_OFF                        = "OFF/horseFur.off";

// Unchangeable definitions.
//> Functions.
static void errorHandler( int error, std::string personalizedText, std::string officialText );
static std::string cuGetErrorString( CUresult cudaResult );
#define                vertices( i, l )            vertices[( i ) * length + ( l )]
#define                internal_vertices( i, l )    vertices[( i ) * length + ( l )]
#define                finalVertices( i, l )        finalVertices[( i ) * length + ( l )]
#define                normals( i, l )                normals[( i ) * length + ( l )]
#define                internal_normals( i, l )    normals[( i ) * length + ( l )]
#define                finalNormals( i, l )        finalNormals[( i ) * length + ( l )]
//> Coordinates.
#define                OX                            1, 0, 0
#define                OY                            0, 1, 0
#define                OZ                            0, 0, 1
#define                O                            0, 0, 0
//> Keys.
const unsigned char KEY_ESC                        = 0x1B;
const unsigned char KEY_SPACE                    = 0x20;
const unsigned char KEY_PLUS                    = 0x2B;
const unsigned char KEY_MINUS                    = 0x2D;
const unsigned char KEY_0                        = 0x30;
const unsigned char KEY_1                        = 0x31;
const unsigned char KEY_2                        = 0x32;
const unsigned char KEY_3                        = 0x33;
const unsigned char KEY_4                        = 0x34;
const unsigned char KEY_5                        = 0x35;
const unsigned char KEY_6                        = 0x36;
const unsigned char KEY_7                        = 0x37;
const unsigned char KEY_8                        = 0x38;
const unsigned char KEY_9                        = 0x39;
const unsigned char KEY_EQUALS                    = 0x3D;
const unsigned char KEY_A_CAPITAL                = 0x41;
const unsigned char KEY_B_CAPITAL                = 0x42;
const unsigned char KEY_C_CAPITAL                = 0x43;
const unsigned char KEY_D_CAPITAL                = 0x44;
const unsigned char KEY_E_CAPITAL                = 0x45;
const unsigned char KEY_F_CAPITAL                = 0x46;
const unsigned char KEY_G_CAPITAL                = 0x47;
const unsigned char KEY_H_CAPITAL                = 0x48;
const unsigned char KEY_I_CAPITAL                = 0x49;
const unsigned char KEY_J_CAPITAL                = 0x4a;
const unsigned char KEY_K_CAPITAL                = 0x4b;
const unsigned char KEY_L_CAPITAL                = 0x4c;
const unsigned char KEY_M_CAPITAL                = 0x4d;
const unsigned char KEY_N_CAPITAL                = 0x4e;
const unsigned char KEY_O_CAPITAL                = 0x4f;
const unsigned char KEY_P_CAPITAL                = 0x50;
const unsigned char KEY_Q_CAPITAL                = 0x51;
const unsigned char KEY_R_CAPITAL                = 0x52;
const unsigned char KEY_S_CAPITAL                = 0x53;
const unsigned char KEY_T_CAPITAL                = 0x54;
const unsigned char KEY_U_CAPITAL                = 0x55;
const unsigned char KEY_V_CAPITAL                = 0x56;
const unsigned char KEY_W_CAPITAL                = 0x57;
const unsigned char KEY_X_CAPITAL                = 0x58;
const unsigned char KEY_Y_CAPITAL                = 0x59;
const unsigned char KEY_Z_CAPITAL                = 0x5a;
const unsigned char KEY_A_LOWER                    = 0x61;
const unsigned char KEY_B_LOWER                    = 0x62;
const unsigned char KEY_C_LOWER                    = 0x63;
const unsigned char KEY_D_LOWER                    = 0x64;
const unsigned char KEY_E_LOWER                    = 0x65;
const unsigned char KEY_F_LOWER                    = 0x66;
const unsigned char KEY_G_LOWER                    = 0x67;
const unsigned char KEY_H_LOWER                    = 0x68;
const unsigned char KEY_I_LOWER                    = 0x69;
const unsigned char KEY_J_LOWER                    = 0x6a;
const unsigned char KEY_K_LOWER                    = 0x6b;
const unsigned char KEY_L_LOWER                    = 0x6c;
const unsigned char KEY_M_LOWER                    = 0x6d;
const unsigned char KEY_N_LOWER                    = 0x6e;
const unsigned char KEY_O_LOWER                    = 0x6f;
const unsigned char KEY_P_LOWER                    = 0x70;
const unsigned char KEY_Q_LOWER                    = 0x71;
const unsigned char KEY_R_LOWER                    = 0x72;
const unsigned char KEY_S_LOWER                    = 0x73;
const unsigned char KEY_T_LOWER                    = 0x74;
const unsigned char KEY_U_LOWER                    = 0x75;
const unsigned char KEY_V_LOWER                    = 0x76;
const unsigned char KEY_W_LOWER                    = 0x77;
const unsigned char KEY_X_LOWER                    = 0x78;
const unsigned char KEY_Y_LOWER                    = 0x79;
const unsigned char KEY_Z_LOWER                    = 0x7a;
/*
// Handle any type of errors given the error number and the readable error string.
static void errorHandler( int error, std::string personalizedText, std::string officialText ){
    HANDLE hStdout;
    // @todo: hStdout = GetStdHandle( STD_OUTPUT_HANDLE );
    // @todo: SetConsoleTextAttribute( hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY );
    if( error == NULL )
        std::cerr << "ERROR N/A: ";
    else
        std::cerr << "ERROR " << error << ": ";
    if( personalizedText.empty() )
        std::cerr << "N/A: ";
    else
        std::cerr << personalizedText << ": ";
    if( officialText.empty() )
        std::cerr << "N/A" << std::endl;
    else
        std::cerr << officialText << std::endl;
    // @todo: SetConsoleTextAttribute( hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
    //std::cin >> personalizedText;
    //exit( error );
}
*/
// Get CUDA error in readable format.
static std::string cuGetErrorString( CUresult cudaResult ){
    switch( cudaResult ){
        case CUDA_SUCCESS:
            return std::string( "CUDA_SUCCESS" );
        case CUDA_ERROR_INVALID_VALUE:
            return std::string( "CUDA_ERROR_INVALID_VALUE" );
        case CUDA_ERROR_OUT_OF_MEMORY:
            return std::string( "CUDA_ERROR_OUT_OF_MEMORY" );
        case CUDA_ERROR_NOT_INITIALIZED:
            return std::string( "CUDA_ERROR_NOT_INITIALIZED" );
        case CUDA_ERROR_DEINITIALIZED:
            return std::string( "CUDA_ERROR_DEINITIALIZED" );
        case CUDA_ERROR_PROFILER_DISABLED:
            return std::string( "CUDA_ERROR_PROFILER_DISABLED" );
        case CUDA_ERROR_PROFILER_NOT_INITIALIZED:
            return std::string( "CUDA_ERROR_PROFILER_NOT_INITIALIZED" );
        case CUDA_ERROR_PROFILER_ALREADY_STARTED:
            return std::string( "CUDA_ERROR_PROFILER_ALREADY_STARTED" );
        case CUDA_ERROR_PROFILER_ALREADY_STOPPED:
            return std::string( "CUDA_ERROR_PROFILER_ALREADY_STOPPED" );
        case CUDA_ERROR_NO_DEVICE:
            return std::string( "CUDA_ERROR_NO_DEVICE" );
        case CUDA_ERROR_INVALID_DEVICE:
            return std::string( "CUDA_ERROR_INVALID_DEVICE" );
        case CUDA_ERROR_INVALID_IMAGE:
            return std::string( "CUDA_ERROR_INVALID_IMAGE" );
        case CUDA_ERROR_INVALID_CONTEXT:
            return std::string( "CUDA_ERROR_INVALID_CONTEXT" );
        case CUDA_ERROR_CONTEXT_ALREADY_CURRENT:
            return std::string( "CUDA_ERROR_CONTEXT_ALREADY_CURRENT" );
        case CUDA_ERROR_MAP_FAILED:
            return std::string( "CUDA_ERROR_MAP_FAILED" );
        case CUDA_ERROR_UNMAP_FAILED:
            return std::string( "CUDA_ERROR_UNMAP_FAILED" );
        case CUDA_ERROR_ARRAY_IS_MAPPED:
            return std::string( "CUDA_ERROR_ARRAY_IS_MAPPED" );
        case CUDA_ERROR_ALREADY_MAPPED:
            return std::string( "CUDA_ERROR_ALREADY_MAPPED" );
        case CUDA_ERROR_NO_BINARY_FOR_GPU:
            return std::string( "CUDA_ERROR_NO_BINARY_FOR_GPU" );
        case CUDA_ERROR_ALREADY_ACQUIRED:
            return std::string( "CUDA_ERROR_ALREADY_ACQUIRED" );
        case CUDA_ERROR_NOT_MAPPED:
            return std::string( "CUDA_ERROR_NOT_MAPPED" );
        case CUDA_ERROR_NOT_MAPPED_AS_ARRAY:
            return std::string( "CUDA_ERROR_NOT_MAPPED_AS_ARRAY" );
        case CUDA_ERROR_NOT_MAPPED_AS_POINTER:
            return std::string( "CUDA_ERROR_NOT_MAPPED_AS_POINTER" );
        case CUDA_ERROR_ECC_UNCORRECTABLE:
            return std::string( "CUDA_ERROR_ECC_UNCORRECTABLE" );
        case CUDA_ERROR_UNSUPPORTED_LIMIT:
            return std::string( "CUDA_ERROR_UNSUPPORTED_LIMIT" );
        case CUDA_ERROR_CONTEXT_ALREADY_IN_USE:
            return std::string( "CUDA_ERROR_CONTEXT_ALREADY_IN_USE" );
        case CUDA_ERROR_INVALID_SOURCE:
            return std::string( "CUDA_ERROR_INVALID_SOURCE" );
        case CUDA_ERROR_FILE_NOT_FOUND:
            return std::string( "CUDA_ERROR_FILE_NOT_FOUND" );
        case CUDA_ERROR_SHARED_OBJECT_SYMBOL_NOT_FOUND:
            return std::string( "CUDA_ERROR_SHARED_OBJECT_SYMBOL_NOT_FOUND" );
        case CUDA_ERROR_SHARED_OBJECT_INIT_FAILED:
            return std::string( "CUDA_ERROR_SHARED_OBJECT_INIT_FAILED" );
        case CUDA_ERROR_OPERATING_SYSTEM:
            return std::string( "CUDA_ERROR_OPERATING_SYSTEM" );
        case CUDA_ERROR_INVALID_HANDLE:
            return std::string( "CUDA_ERROR_INVALID_HANDLE" );
        case CUDA_ERROR_NOT_FOUND:
            return std::string( "CUDA_ERROR_NOT_FOUND" );
        case CUDA_ERROR_NOT_READY:
            return std::string( "CUDA_ERROR_NOT_READY" );
        case CUDA_ERROR_LAUNCH_FAILED:
            return std::string( "CUDA_ERROR_LAUNCH_FAILED" );
        case CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES:
            return std::string( "CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES" );
        case CUDA_ERROR_LAUNCH_TIMEOUT:
            return std::string( "CUDA_ERROR_LAUNCH_TIMEOUT" );
        case CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING:
            return std::string( "CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING" );
        case CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED:
            return std::string( "CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED" );
        case CUDA_ERROR_PEER_ACCESS_NOT_ENABLED:
            return std::string( "CUDA_ERROR_PEER_ACCESS_NOT_ENABLED" );
        case CUDA_ERROR_PRIMARY_CONTEXT_ACTIVE:
            return std::string( "CUDA_ERROR_PRIMARY_CONTEXT_ACTIVE" );
        case CUDA_ERROR_CONTEXT_IS_DESTROYED:
            return std::string( "CUDA_ERROR_CONTEXT_IS_DESTROYED" );
        case CUDA_ERROR_UNKNOWN:
            return std::string( "CUDA_ERROR_UNKNOWN" );
        default:
            return std::string( "cuGetErrorString() doesn't recognize error." );
    }
}

#endif
