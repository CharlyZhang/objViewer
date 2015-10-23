
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

// Scene
typedef struct _CZScene {
	CZColor	bgColor;		//< ������ɫ
	CZColor mColor;			//< ģ����ɫ
	CZLight	light;			//< ��Դ
	CZPoint3D eyePosition;	//< �ӵ�λ��
} CZScene;

#endif