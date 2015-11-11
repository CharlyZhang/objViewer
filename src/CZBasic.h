
#ifndef _CZBASIC_H_
#define _CZBASIC_H_

#include "CZVector.h"

// 3D Point
typedef CZVector3D<float> CZPoint3D;

// Color
typedef struct _CZColor {
	float r,g,b,a;
	_CZColor(float r_=0.0,float g_=0.0,float b_=0.0,float a_=0.0){r=r_; g=g_; b=b_; a=a_;}
} CZColor;

// Light
typedef struct _CZLight {
	CZPoint3D position;
	CZPoint3D intensity;
} CZLight;

typedef struct _CZAmbientLight {
	CZPoint3D intensity;
} CZAmbientLight;

typedef struct _CZDirectionalLight {
	CZPoint3D direction;
	CZPoint3D intensity;
} CZDirectionalLight;

// Scene
typedef struct _CZScene {
	CZColor	bgColor;                        //< background color
	CZColor mColor;							//< model color
	CZLight	light;							//< point light
	CZAmbientLight ambientLight;			//< ambient light
	CZDirectionalLight directionalLight;	//< diretional light
	CZPoint3D eyePosition;					//< eye position
} CZScene;

#endif