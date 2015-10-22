
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

#endif