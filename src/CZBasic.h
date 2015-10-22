
#ifndef _CZBASIC_H_
#define _CZBASIC_H_

#include "CZVector.h"

typedef CZVector3D<float> CZPoint3D;

// Color
typedef struct _CZColor {
	float r,g,b,a;
} CZColor;

// Light
typedef struct _CZLight {
	CZPoint3D position;
	CZPoint3D intensity;
} CZLight;

// Camera
typedef struct _CZCamera {
	CZPoint3D eye;
	CZPoint3D center;
	CZPoint3D up;

	void set(float eyeX, float eyeY, float eyeZ,
		float centerX, float centerY, float centerZ,
		float upX, float upY, float upZ)
	{
		eye = CZPoint3D(eyeX,eyeY,eyeZ);
		center = CZPoint3D(centerX,centerY,centerZ);
		up = CZPoint3D(upX,upY,upZ);
	}
} CZCamera;

#endif