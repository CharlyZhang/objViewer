
#include "CZBasic.h"
#include "CZObjModel.h"

class CZScene 
{
public:
	CZColor	bgColor;		//< ������ɫ
	CZColor mColor;			//< ģ����ɫ
	CZLight	light;			//< ��Դ
	CZCamera camera;		//< ���
	CZObjModel model;		//< ģ��
};