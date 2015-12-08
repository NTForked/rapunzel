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
/                                      |  |
/                                      |--
/                                      |
/                                      V z
/-------------------------------------------------------------------------------
*/

#ifndef CAMERA_H
#define CAMERA_H

#pragma once

#include "HeadersAndDefines.h"

class Camera : public Object3D{
public:
	// Fields.
	Vector3D forward;
	Vector3D right;
	Vector3D up;
	Vector3D position;

	// Constructor.
	Camera();

	// Place observer in scene.
	void render();
	
	// Rotations.
	void checkAngle( float &angle);
	void rotateFPS_OX( float angle );
	void rotateFPS_OY( float angle );
	void rotateTPS_OX( float angle, float distance );
	void rotateTPS_OY( float angle, float distance );

	// Translations.
	void setPosition ( Vector3D coordinates );
	void moveForward( GLfloat distance );
	void moveRight ( GLfloat distance );
	void moveUpward ( GLfloat distance );
	void moveForwardGrounded( GLfloat distance );
	void moveRightGrounded( GLfloat distance );

	void lookAt( Vector3D point );
};

#endif
