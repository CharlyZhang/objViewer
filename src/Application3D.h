#ifndef _CZAPPLICATION3D_H_
#define _CZAPPLICATION3D_H_

#include <string>
#include "CZScene.h"
#include "CZShader.h"
#include "CZMat4.h"

class Application3D 
{
public:
	Application3D();
	~Application3D();

	bool init();
	bool setRenderBufferSize(int w, int h);
	bool loadScene(const std::string &filename);
	void frame();
	void reset();

	//control
	//	/note : (deltaX,deltaY) is in the screen coordinate system
	void rotate(float deltaX, float deltaY);
	void translate(float deltaX, float deltaY);

	// custom config
	bool loadObjModel(const std::string &filename);
	void setBackgroundColor(float r, float g, float b, float a);
	void setModelColor(float r, float g, float b, float a);
	// eye position
	void setEyePosition(float x, float y, float z);
	// light
	void setLightPosition(float x, float y, float z);
	void setLightDiffuse(float r, float g, float b, float a);

	// texture
	bool enableTexture(bool flag);

private:
	CZScene *pScene;
	CZShader *pShader;
	CZMat4 projMat,rotateMat;
	int width, height;
};

#endif