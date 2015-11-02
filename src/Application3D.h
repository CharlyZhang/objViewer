#ifndef _CZAPPLICATION3D_H_
#define _CZAPPLICATION3D_H_

#include <string>
#include "CZBasic.h"
#include "CZObjModel.h"
#include "CZShader.h"
#include "CZMat4.h"

class Application3D 
{
public:
	Application3D();
	~Application3D();

	bool init();
	bool loadObjModel(const std::string &filename);
    bool loadObjModel(const char *filename);
	bool setRenderBufferSize(int w, int h);
	void frame();
	void reset();

	// control
	//	/note : (deltaX,deltaY) is in the screen coordinate system
	void rotate(float deltaX, float deltaY);
	void translate(float deltaX, float deltaY);
	void scale(float s);

	// custom config
	void setBackgroundColor(float r, float g, float b, float a);
	void setModelColor(float r, float g, float b, float a);
	// light
	void setLightPosition(float x, float y, float z);		///< TO DEPRECATED
	// texture
	bool enableTexture(bool flag);

private:
	bool loadScene();

private:
	CZScene scene;				
	CZObjModel *pModel;
	CZShader *pShader;
	CZMat4 projMat,rotateMat,translateMat,scaleMat;
	int width, height;
	CZColor modelColor;
};

#endif