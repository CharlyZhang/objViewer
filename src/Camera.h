#ifndef _CAMERA_H
#define _CAMERA_H

#include "CZVector.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string>										// Used for our STL string objects
#include <fstream>	
// Used for our ifstream object to load text files

#define  SCREEN_WIDTH	800
#define  SCREEN_HEIGHT	600

// This is our camera class
class CCamera {

public:
	// Our camera constructor
	CCamera();	

	// These are are data access functions for our camera's private data
	CZVector3D<float> Position() {	return m_vPosition;		}
	CZVector3D<float> View()		{	return m_vView;			}
	CZVector3D<float> UpVector() {	return m_vUpVector;		}
	CZVector3D<float> Strafe()	{	return m_vStrafe;		}
	
	// This changes the position, view, and up vector of the camera.
	// This is primarily used for initialization
	void PositionCamera(float positionX, float positionY, float positionZ,
			 		    float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ);
	void PositionCamera(CZVector3D<float> &pos, CZVector3D<float> &view, CZVector3D<float> &up);

	// This rotates the camera's view around the position depending on the values passed in.
	void RotateView(float angle, float X, float Y, float Z);

	// This moves the camera's view by the mouse movements (First person view)
	void SetViewByMouse(); 

	// This rotates the camera around a point (I.E. your character).
	void RotateAroundPoint(CZVector3D<float> vCenter, float X, float Y, float Z);

	// This strafes the camera left or right depending on the speed (+/-) 
	void StrafeCamera(float speed);

	// This will move the camera forward or backward depending on the speed
	void MoveCamera(float speed);

	// This checks for keyboard movement
	void CheckForMovement();

	// This updates the camera's view and other data (Should be called each frame)
	void Update();

	// This uses gluLookAt() to tell OpenGL where to look
	void Look();

	// This returns the inverse to the current modelview matrix in OpenGL
	void GetInverseMatrix(float mCameraInverse[16]);

private:

	// The camera's position
	CZVector3D<float> m_vPosition;					

	// The camera's view
	CZVector3D<float> m_vView;						

	// The camera's up vector
	CZVector3D<float> m_vUpVector;		
	
	// The camera's strafe vector
	CZVector3D<float> m_vStrafe;						
};
//数值定义
#define PI 3.14159265358979323846
#define EE 2.71828182845904523536

extern double angle;
// This makes sure we only draw at a specified frame rate
bool AnimateNextFrame(int desiredFrameRate);

//漫游函数
void AirRoam(void);
#endif


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Nothing new was added to this file for our current tutorial.
// 
// 
// ?000-2005 GameTutorials