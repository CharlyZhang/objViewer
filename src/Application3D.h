#ifndef _CZAPPLICATION3D_H_
#define _CZAPPLICATION3D_H_

#include <string>
#include "CZScene.h"
#include "CZShader.h"
#include "CZMat4.h"
#include "Camera.h"

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

	// custom config
	bool loadObjModel(const std::string &filename);
	void setBackgroundColor(float r, float g, float b, float a);
	void setModelColor(float r, float g, float b, float a);
	// camera
	void setCamera(float eyeX, float eyeY, float eyeZ,
					float centerX, float centerY, float centerZ,
					float upX, float upY, float upZ);
	// camera
	void setCamera(CCamera *c);
	// light
	void setLightPosition(float x, float y, float z);
	void setLightDiffuse(float r, float g, float b, float a);

	// texture
	bool enableTexture(bool flag);

private:
	CZScene *pScene;
	CZShader *pShader;
	CZMat4 projMat;
	int width, height;
	CCamera *pCamera;
};

#endif