
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
	CZColor	bgColor;						//< 背景颜色
	CZColor mColor;							//< 模型颜色
	CZLight	light;							//< 点光源
	CZAmbientLight ambientLight;			//< 环境光
	CZDirectionalLight directionalLight;	//< 平行光
	CZPoint3D eyePosition;					//< 视点位置
} CZScene;

#endif