
///  \file CZTexture.h
///  \brief This is the file declare the Class CZTexture.
///
///		This file includes CZTexture, and all are about 2D.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-19
///  \note

#ifndef _CZTEXTURE_H_
#define _CZTEXTURE_H_

#include "../basic/CZImage.h"
#include "../CZDefine.h"

class CZTexture
{
public:
	CZTexture(int width_, int height_, StorageMode mode_ = DEFAULT_STORAGEMODE, void *data = 0);
	~CZTexture();
	/// ��CZImage��������
	static CZTexture* produceFromImage(CZImage *img);
	/// ��ͼƬ�����޸�����
	bool modifyWith(CZImage *img,int x = 0, int y = 0);

	/// ��ȡͼƬģʽ
	StorageMode getMode() {return mode;};

	/// ��ȡ���Ӧ��ͼ������
	CZImage *toImage();
	/// �������Բ�ֵ
	void enableLinearInterprolation(float flag);

public:
	unsigned int texId;
	int width,height;
	StorageMode mode;
};
#endif