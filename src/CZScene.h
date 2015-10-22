
#include "CZBasic.h"
#include "CZObjModel.h"

class CZScene 
{
public:
	CZColor	bgColor;		//< 背景颜色
	CZColor mColor;			//< 模型颜色
	CZLight	light;			//< 光源
	CZCamera camera;		//< 相机
	CZObjModel model;		//< 模型
};