#ifndef _CZAPPLICATION3D_H_
#define _CZAPPLICATION3D_H_

#include <string>
#include <map>
#include "CZBasic.h"
#include "CZObjFileParser.h"
#include "CZShader.h"
#include "CZMat4.h"
#include "CZObjModel.h"

class Application3D : private CZObjFileParser
{
public:
	// define type
	typedef enum _ShaderType {
		kDirectionalLightShading		///< 平行光光照
	} ShaderType;
	typedef std::map<ShaderType,CZShader*> ShaderMap;

	Application3D();
	~Application3D();

	bool init(const char* sceneFilename = NULL);
	bool loadObjModel(const char* filename, bool quickLoad = true);
	bool setRenderBufferSize(int w, int h);
	void frame();
	void reset();
    
    // document directory
    //  /note : default as the same of model's location;
    //          should be set in ios platform to utilize binary data 
    void setDocDirectory(const char* docDir);
    
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
	bool loadShaders();
	CZShader* getShader(ShaderType type);

	void parseLine(std::ifstream& ifs, const std::string& ele_id) override;
	void parseEyePosition(std::ifstream& ifs);
	void parsePointLight(std::ifstream& ifs);
	void parseDirectionalLight(std::ifstream& ifs);
	void parseBackgroundColor(std::ifstream& ifs);
	void parseMainColor(std::ifstream& ifs);

private:
	CZScene scene;
	ShaderMap shaders;
	CZObjModel *pModel;
	CZMat4 projMat, rotateMat, translateMat, scaleMat;
	int width, height;
	CZColor modelColor;
    
    char *documentDirectory;                          ///< to store the binary data of model
};

#endif