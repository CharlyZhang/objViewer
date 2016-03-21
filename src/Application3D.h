#ifndef _CZAPPLICATION3D_H_
#define _CZAPPLICATION3D_H_

#include <string>
#include <map>
#include "CZBasic.h"
#include "CZObjFileParser.h"
#include "CZShader.h"
#include "CZMat4.h"
#include "CZObjModel.h"

typedef std::vector<CZObjModel*> CZObjModelArray;
typedef std::vector<CZMat4> CZMat4Array;

class Application3D : private CZObjFileParser
{
public:
	// define type
	typedef enum _ShaderType {
		kDirectionalLightShading		///< ƽ�й����
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
    
    void setGLSLDirectory(const char* glslDir);
    
	// control
	//	/note : (deltaX,deltaY) is in the screen coordinate system
	void rotate(float deltaX, float deltaY, int modelIdx = -1);
	void translate(float deltaX, float deltaY, float deltaZ = 0.0f, int modelIdx = -1);
	void scale(float s, int modelIdx = -1);

	// custom config
	void setBackgroundColor(float r, float g, float b, float a);
	void setModelColor(float r, float g, float b, float a);
    
    // camera
    void setCameraPosition(float x, float y, float z);
    
	// light
	void setLightPosition(float x, float y, float z);   ///< TO DEPRECATED
    void setLigthDirection(float x, float y, float z);
    void setAmbientColor(unsigned char r, unsigned char g, unsigned char b);
    void setDiffuseColor(unsigned char r, unsigned char g, unsigned char b);
    
	// texture
	bool enableTexture(bool flag);

private:
	bool loadShaders();
	CZShader* getShader(ShaderType type);

	void parseLine(std::ifstream& ifs, const std::string& ele_id) override;
	void parseEyePosition(std::ifstream& ifs);
    void parseCameraFov(std::ifstream& ifs);
    void parseCameraNearPlane(std::ifstream& ifs);
    void parseCameraFarPlane(std::ifstream& ifs);
	void parsePointLight(std::ifstream& ifs);
	void parseDirectionalLight(std::ifstream& ifs);
	void parseBackgroundColor(std::ifstream& ifs);
	void parseMainColor(std::ifstream& ifs);

private:
	CZScene scene;
	ShaderMap shaders;
    CZObjModelArray models;
    CZMat4Array rotateMats, translateMats, scaleMats;
	CZMat4 projMat,entireTranslageMat;
    
	int width, height;
	CZColor modelColor;
    
    char *documentDirectory;                          ///< to store the binary data of model
};

#endif