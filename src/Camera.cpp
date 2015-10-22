//*********************************************************************//
//																		 //
//		- "Talk to me like I'm a 3 year old!" Programming Lessons -		 //
//                                                                       //
//		$Author:		Ben Humphrey	digiben@gametutorilas.com		 //
//																		 //
//		$Program:		ShadersShadowMappingPCF							 //
//																		 //
//		$Description:	Creating higher quality shadowmaps with GLSL	 //
//																		 //
//***********************************************************************//

#pragma comment(lib, "winmm.lib")

#include "Camera.h"
#include <gl\gl.h>										// Header File For The OpenGL32 Library
#include <gl\glu.h>										// Header File For The GLu32 Library
#include "glut.h"
#include <iostream>
using namespace std;
double angle;
// We increased the speed a bit from the Camera Strafing Tutorial
// This is how fast our camera moves
#define kSpeed	250.0f									

// Our global float that stores the elapsed time between the current and last frame
float g_FrameInterval = 0.0f;


/////////////////////////////// ANIMATE NEXT FRAME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function clamps a specified frame rate for consistency
/////
/////////////////////////////// ANIMATE NEXT FRAME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool AnimateNextFrame(int desiredFrameRate)
{
	static double lastTime = 0.0f;
	double elapsedTime = 0.0;

	// Get current time in seconds  (milliseconds * .001 = seconds)
    double currentTime = timeGetTime() * 0.001; 

	// Get the elapsed time by subtracting the current time from the last time
	elapsedTime = currentTime - lastTime;

	// Check if the time since we last checked is over (1 second / framesPerSecond)
    if( elapsedTime > (1.0 / desiredFrameRate) )
    {
		// Reset the last time
        lastTime = currentTime;	

		// Return TRUE, to animate the next frame of animation
        return true;
    }

	// We don't animate right now.
	return false;
}


///////////////////////////////// CALCULATE FRAME RATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function calculates the frame rate and time intervals between frames
/////
///////////////////////////////// CALCULATE FRAME RATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CalculateFrameRate()
{
	static float framesPerSecond    = 0.0f;		// This will store our fps
    static float lastTime			= 0.0f;		// This will hold the time from the last frame
	static char strFrameRate[50] = {0};			// We will store the string here for the window title

	static float frameTime = 0.0f;				// This stores the last frame's time

	// Get the current time in seconds
    float currentTime = timeGetTime() * 0.001f;				

	// Here we store the elapsed time between the current and last frame,
	// then keep the current frame in our static variable for the next frame.
 	g_FrameInterval = currentTime - frameTime;
	frameTime = currentTime;

	// Increase the frame counter
    ++framesPerSecond;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
    if( currentTime - lastTime > 1.0f )
    {		
		//// Copy the frames per second into a string to display in the window title bar
		sprintf(strFrameRate, "Current Frames Per Second: %0.2f", framesPerSecond/(currentTime - lastTime));

		//// Set the window title bar to our string
		glutSetWindowTitle(strFrameRate);

		// Here we set the lastTime to the currentTime
		lastTime = currentTime;

		// Reset the frames per second
        framesPerSecond = 0;
    }
}

///////////////////////////////// CCAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This is the class constructor
/////
///////////////////////////////// CCAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CCamera::CCamera()
{
	CZVector3D<float> vZero = CZVector3D<float>(0.0, 0.0, 0.0);		// Init a vector to 0 0 0 for our position
	CZVector3D<float> vView = CZVector3D<float>(0.0, 1.0, 0.5);		// Init a starting view vector (looking up and out the screen) 
	CZVector3D<float> vUp   = CZVector3D<float>(0.0, 0.0, 1.0);		// Init a standard up vector (Rarely ever changes)

	m_vPosition	= vZero;					// Init the position to zero
	m_vView		= vView;					// Init the view to a std starting view
	m_vUpVector	= vUp;						// Init the UpVector

	// Set the mouse position to the middle of our window
	SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);							
}


///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function sets the camera's position and view and up vector.
/////
///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::PositionCamera(float positionX, float positionY, float positionZ,
				  		     float viewX,     float viewY,     float viewZ,
							 float upVectorX, float upVectorY, float upVectorZ)
{
	CZVector3D<float> vPosition	= CZVector3D<float>(positionX, positionY, positionZ);
	CZVector3D<float> vView		= CZVector3D<float>(viewX, viewY, viewZ);
	CZVector3D<float> vUpVector	= CZVector3D<float>(upVectorX, upVectorY, upVectorZ);

	// The code above just makes it cleaner to set the variables.
	// Otherwise we would have to set each variable x y and z.

	m_vPosition = vPosition;					// Assign the position
	m_vView     = vView;						// Assign the view
	m_vUpVector = vUpVector;					// Assign the up vector
}
void CCamera::PositionCamera(CZVector3D<float> &pos, CZVector3D<float> &view, CZVector3D<float> &up)
{
	// The code above just makes it cleaner to set the variables.
	// Otherwise we would have to set each variable x y and z.

	m_vPosition = pos;					// Assign the position
	m_vView     = view;						// Assign the view
	m_vUpVector = up;					// Assign the up vector
}


///////////////////////////////// SET VIEW BY MOUSE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This allows us to look around using the mouse, like in most first person games.
/////
///////////////////////////////// SET VIEW BY MOUSE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::SetViewByMouse()
{
	POINT mousePos;									// This is a window structure that holds an X and Y
	int middleX = SCREEN_WIDTH  >> 1;				// This is a binary shift to get half the width
	int middleY = SCREEN_HEIGHT >> 1;				// This is a binary shift to get half the height
	float angleY = 0.0f;							// This is the direction for looking up or down
	float angleZ = 0.0f;							// This will be the value we need to rotate around the Y axis (Left and Right)
	static float currentRotX = 0.0f;
	
	// Get the mouse's current X,Y position
	GetCursorPos(&mousePos);		
	
	
	// If our cursor is still in the middle, we never moved... so don't update the screen
	if( (mousePos.x == middleX) && (mousePos.y == middleY) ) return;

	// Set the mouse position to the middle of our window
	SetCursorPos(middleX, middleY);	


	// Get the direction the mouse moved in, but bring the number down to a reasonable amount
	angleY = (float)( (middleX - mousePos.x) ) / 200.0f;		
	angleZ = (float)( (middleY - mousePos.y) ) / 200.0f;		

	static float lastRotX = 0.0f; 
 	lastRotX = currentRotX; // We store off the currentRotX and will use it in when the angle is capped
	
	// Here we keep track of the current rotation (for up and down) so that
	// we can restrict the camera from doing a full 360 loop.
	currentRotX += angleZ;
 
	// If the current rotation (in radians) is greater than 1.0, we want to cap it.
	if(currentRotX > 1.0f)     
	{
		currentRotX = 1.0f;
		
		// Rotate by remaining angle if there is any
		if(lastRotX != 1.0f) 
		{
			// To find the axis we need to rotate around for up and down
			// movements, we need to get a perpendicular vector from the
			// camera's view vector and up vector.  This will be the axis.
			// Before using the axis, it's a good idea to normalize it first.
			CZVector3D<float> vAxis = (m_vView - m_vPosition).cross(m_vUpVector);
			vAxis.normalize();
				
			// rotate the camera by the remaining angle (1.0f - lastRotX)
			RotateView( 1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	// Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
	else if(currentRotX < -1.0f)
	{
		currentRotX = -1.0f;
		
		// Rotate by the remaining angle if there is any
		if(lastRotX != -1.0f)
		{
			// To find the axis we need to rotate around for up and down
			// movements, we need to get a perpendicular vector from the
			// camera's view vector and up vector.  This will be the axis.
			// Before using the axis, it's a good idea to normalize it first.
			CZVector3D<float> vAxis = (m_vView - m_vPosition).cross(m_vUpVector);
			vAxis.normalize();
			
			// rotate the camera by ( -1.0f - lastRotX)
			RotateView( -1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	// Otherwise, we can rotate the view around our position
	else 
	{	
		// To find the axis we need to rotate around for up and down
		// movements, we need to get a perpendicular vector from the
		// camera's view vector and up vector.  This will be the axis.
		// Before using the axis, it's a good idea to normalize it first.
		CZVector3D<float> vAxis = (m_vView - m_vPosition).cross(m_vUpVector);
		vAxis.normalize();
	
		// Rotate around our perpendicular axis
		RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}

	// Always rotate the camera around the y-axis
	RotateView(angleY, 0, 1, 0);
}


///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This rotates the view around the position using an axis-angle rotation
/////
///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::RotateView(float angle, float x, float y, float z)
{
	CZVector3D<float> vNewView;

	// Get the view vector (The direction we are facing)
	CZVector3D<float> vView = m_vView - m_vPosition;		

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	// Find the new y position for the new rotated point
	vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	// Find the new z position for the new rotated point
	vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	m_vView = m_vPosition + vNewView;
}


///////////////////////////////// STRAFE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This strafes the camera left and right depending on the speed (-/+)
/////
///////////////////////////////// STRAFE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::StrafeCamera(float speed)
{	
	// Add the strafe vector to our position
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;

	// Add the strafe vector to our view
	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;
}


///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This will move the camera forward or backward depending on the speed
/////
///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::MoveCamera(float speed)
{
	// Get the current view vector (the direction we are looking)
	CZVector3D<float> vVector = m_vView - m_vPosition;
	vVector.normalize();

	m_vPosition.x += vVector.x * speed;		// Add our acceleration to our position's X
	m_vPosition.y += vVector.y * speed;		// Add our acceleration to our position's Y
	m_vPosition.z += vVector.z * speed;		// Add our acceleration to our position's Z
	m_vView.x += vVector.x * speed;			// Add our acceleration to our view's X
	m_vView.y += vVector.y * speed;			// Add our acceleration to our view's Y
	m_vView.z += vVector.z * speed;			// Add our acceleration to our view's Z
}


//////////////////////////// CHECK FOR MOVEMENT \\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles the input faster than in the WinProc()
/////
//////////////////////////// CHECK FOR MOVEMENT \\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::CheckForMovement()
{	
	// Once we have the frame interval, we find the current speed
	float speed = kSpeed * g_FrameInterval;

	// Check if we hit the Up arrow or the 'w' key
	if(GetKeyState('W') & 0x80) {				

		// Move our camera forward by a positive SPEED
		MoveCamera(speed);				
	}

	// Check if we hit the Down arrow or the 's' key
	if(GetKeyState('S') & 0x80) {			

		// Move our camera backward by a negative SPEED
		MoveCamera(-speed);				
	}

	// Check if we hit the Left arrow or the 'a' key
	if(GetKeyState('A') & 0x80) {			

		// Strafe the camera left
		StrafeCamera(-speed);
	}

	// Check if we hit the Right arrow or the 'd' key
	if(GetKeyState('D') & 0x80) {			

		// Strafe the camera right
		StrafeCamera(speed);
	}	
}


///////////////////////////////// UPDATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This updates the camera's view and strafe vector
/////
///////////////////////////////// UPDATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::Update( ) 
{
	// Initialize a variable for the cross product result
	CZVector3D<float> vCross = (m_vView - m_vPosition).cross(m_vUpVector);

	// Normalize the strafe vector
	vCross.normalize();
	m_vStrafe = vCross;

	// Move the camera's view by the mouse
	SetViewByMouse();

	// This checks to see if the keyboard was pressed
	CheckForMovement();
	
	// Calculate our frame rate and set our frame interval for time-based movement
	CalculateFrameRate();
}


///////////////////////////////// LOOK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This updates the camera according to the 
/////
///////////////////////////////// LOOK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::Look()
{
	// Give OpenGL our camera position, then camera view, then camera up vector
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,	
			  m_vView.x,	 m_vView.y,     m_vView.z,	
			  m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}


/////////////////////////////// GET INVERSE MATRIX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the inverse to the current modelview matrix in OpenGL
/////
/////////////////////////////// GET INVERSE MATRIX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::GetInverseMatrix(float mCameraInverse[16])
{
	float m[16] = {0};

	// Grab the current modelview matrix from OpenGL
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	// Below we do a little algebra to invert our current modelview matrix.
	// Instead of doing a more complicated method of determining the inverse
	// of a matrix, we can use this simplified version because we assume our
	// matrix is an orthogonal matrix, meaning that each column is perpendicular
	// to the other column.  What we do is remove the translation of the matrix
	// and then multiply it later by the negative translation, which reverses the
	// calculations of the original matrix.
	mCameraInverse[0]  = m[0]; mCameraInverse[1] = m[4]; mCameraInverse[2]  = m[8];
	mCameraInverse[4]  = m[1]; mCameraInverse[5] = m[5]; mCameraInverse[6]  = m[9];
	mCameraInverse[8]  = m[2]; mCameraInverse[9] = m[6]; mCameraInverse[10] = m[10];
	mCameraInverse[3]  = 0.0f; mCameraInverse[7] = 0.0f; mCameraInverse[11] = 0.0f;
	mCameraInverse[15] = 1.0f;

	mCameraInverse[12] = -(m[12] * m[0]) - (m[13] * m[1]) - (m[14] * m[2]);
	mCameraInverse[13] = -(m[12] * m[4]) - (m[13] * m[5]) - (m[14] * m[6]);
	mCameraInverse[14] = -(m[12] * m[8]) - (m[13] * m[9]) - (m[14] * m[10]);
}

//漫游函数
void AirRoam(void)
{
	// 初始观察点的位置
	static GLfloat eyePosition[]    ={ 0.0, 0.0, 7.0 };
	// 初始观察点的观察方向
	static GLfloat watchDirection[] ={ 0.0, 0.0, 0.0 };
	// xz平面的角度，x正方向为0，z正方向为-90
	static GLfloat xz_angle = -90.0;
	// zy平面的角度，z负方向为0，y正方向为90
	static GLfloat zy_angle = 0.0;
	// 移动速度
	double speed = 0.01;

	// 旋转请求
	if(GetKeyState('A') & 0x80)
	{
		xz_angle -= 1.0;		                //每按一次左键，旋转2度。
	}
	if(GetKeyState('D') & 0x80)
	{
		xz_angle += 1.0;                       //每按一次右键，旋转2度。
	} 
	float rad =float(PI*xz_angle/180.0f);                    //计算SIN和COS函数中需要的参数。

	//视点向上向下移动请求
	if(GetKeyState('Q') & 0x80)
	{
		zy_angle += 1.0;
	}
	if(GetKeyState('E') & 0x80)
	{
		zy_angle -= 1.0;
	}
	float al =float(PI*zy_angle/180.0f);

	// 前进,后退请求，更新观察点位置
	if(GetKeyState('W') & 0x80)
	{
		eyePosition[2] += 5.0*(float)sin(rad)*cos(al) * speed;
		eyePosition[0] += 5.0*(float)cos(rad)*cos(al) * speed;
		eyePosition[1] += 5.0*(float)sin(al) * speed;
		//如果按 W 键，沿着转换角度后的方向前进，speed为每次前进的步长，通过sin和cos函数实现沿着现
		//有角度方向前进。
	}
	if(GetKeyState('S') & 0x80)
	{
		eyePosition[2] -= 5.0*(float)sin(rad)*cos(al) * speed;
		eyePosition[0] -= 5.0*(float)cos(rad)*cos(al) * speed;
		eyePosition[1] -= 5.0*(float)sin(al) * speed;
		//如果按 S 键，沿着转换角度后的方向后退，speed为每次前进的步长，通过sin和cos函数实现沿着现
		//有角度方向前进。
	}

	// 前进,后退请求，更新观察点位置
	if(GetKeyState(VK_UP) & 0x80 )
	{
		eyePosition[2] += 5.0*(float)sin(rad)*cos(al) * speed;
		eyePosition[0] += 5.0*(float)cos(rad)*cos(al) * speed;
		//如果按上方向键，沿着水平方向前进，speed为每次前进的步长，通过sin和cos函数实现沿着现
		//有角度方向前进。
	}
	if(GetKeyState(VK_DOWN) & 0x80)
	{
		eyePosition[2] -= 5.0*(float)sin(rad)*cos(al) * speed;
		eyePosition[0] -= 5.0*(float)cos(rad)*cos(al) * speed;
		//如果按下方向键，沿着水平方向后退，speed为每次前进的步长，通过sin和cos函数实现沿着现
		//有角度方向前进。
	}

	// 前进,后退请求，更新观察点位置
	if(GetKeyState(VK_LEFT) & 0x80)
	{
		eyePosition[2] += 5.0*(float)sin(rad-PI/2)*cos(al) * speed;
		eyePosition[0] += 5.0*(float)cos(rad-PI/2)*cos(al) * speed;
		//如果按左方向键，沿着水平方向左移，speed为每次前进的步长，通过sin和cos函数实现沿着现
		//有角度方向前进。
	}
	if(GetKeyState(VK_RIGHT) & 0x80)
	{
		eyePosition[2] -= 5.0*(float)sin(rad-PI/2)*cos(al) * speed;
		eyePosition[0] -= 5.0*(float)cos(rad-PI/2)*cos(al) * speed;
		//如果按右方向键，沿着水平方向右移，speed为每次前进的步长，通过sin和cos函数实现沿着现
		//有角度方向前进。
	}


	// 更新观察方向
	watchDirection[0] = float(eyePosition[0] + 100*cos(rad)*cos(al) );    
	watchDirection[2] = float(eyePosition[2] + 100*sin(rad)*cos(al) );
	watchDirection[1] = float(eyePosition[1] + 100*sin(al) );

	// lookAt
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt(eyePosition[0], eyePosition[1], eyePosition[2],
		watchDirection[0], watchDirection[1], watchDirection[2],
		0.0, 1.0, 0.0);
}


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Nothing new was added to this file for our current tutorial.
//
//
// ?000-2005 GameTutorials