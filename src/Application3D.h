#ifndef _CZAPPLICATION3D_H_
#define _CZAPPLICATION3D_H_

#include <string>
#include <map>
#include "CZBasic.h"
#include "CZObjFileParser.h"
#include "CZShader.h"
#include "CZMat4.h"
#include "CZNode.h"
#include "CZObjModel.h"
#include "animation/CZAnimaitonManager.hpp"

class Application3D : private CZObjFileParser
{
public:
	// define type
	typedef enum _ShaderType {
		kDirectionalLightShading,		///< directional light shadding mode
        kBlitImage,                      ///< blit image to the renderbuffer
        kBlitColor                      ///< blit color
	} ShaderType;
	typedef std::map<ShaderType,CZShader*> ShaderMap;

	Application3D();
	~Application3D();

	bool init(const char* sceneFilename = NULL);
	bool loadObjModel(const char* filename, bool quickLoad = true);
    bool clearObjModel();
	bool setRenderBufferSize(int w, int h);
    // frame
    //  called to draw the scene
    //  \note `nowTime` is only necessary when `animation` is added
	void frame(double nowTime = 0.0f);
	void reset();
    
    // shape
    bool createShape(const char* shapeFileName, bool contentInParam = false);
    bool clearShapes();
    bool animateShape(const char* shapeName, const char* animName, double nowTime);

#ifdef	__ANDROID__
	bool createShader(ShaderType type,const char* vertFile, const char* fragFile, std::vector<std::string> &attributes,std::vector<std::string> &uniforms);
    void setImageLoader(const char * cls, const char * method);
    void setModelLoadCallBack(const char * cls, const char *method);
#endif
    
    // document directory
    //  /note : default as the same of model's location;
    //          should be set in ios platform to utilize binary data 
    void setDocDirectory(const char* docDir);
    
    void setGLSLDirectory(const char* glslDir);
    
	// control
	//	/note : (deltaX,deltaY) is in the screen coordinate system
	void rotate(float deltaX, float deltaY, const char *nodeName = nullptr);
	void translate(float deltaX, float deltaY, const char *nodeName = nullptr);
	void scale(float s, const char *nodeName = nullptr);

	// custom config
	void setBackgroundColor(float r, float g, float b, float a);
    void setBackgroundImage(CZImage *img);
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
    
    bool blitBackgroundImage();

private:
	CZScene scene;
	ShaderMap shaders;
    CZNode rootNode;
	CZMat4 projMat;
    CZAnimationManager animationManager;
    
	int width, height;
	CZColor modelColor;
    CZImage *backgroundImage;
    GLuint backgroundTexId,vao;;
    char *documentDirectory;                          ///< to store the binary data of model
};

#endif