#include "Application3D.h"
#include "CZDefine.h"
#include "CZGeometry.h"
#include "CZLog.h"
#include <vector>
#include <string>

#define DEFAULT_RENDER_SIZE 500					///< 默认渲染缓存大小
#define CONFIG_FILE_PATH	"./scene.cfg"

using namespace std;

Application3D::Application3D()
{
	width = height = DEFAULT_RENDER_SIZE;
	pModel = NULL;
    documentDirectory = NULL;
}

Application3D::~Application3D()
{
	if (pModel) { delete pModel; pModel = NULL; }

	for (map<ShaderType,CZShader*>::iterator itr = shaders.begin(); itr != shaders.end(); itr++)
	{
		delete itr->second;
	}
	shaders.clear();
    if(documentDirectory)   delete documentDirectory;
}

bool Application3D::init(const char* sceneFilename /* = NULL */ )
{
# if !defined(__APPLE__)
	/// OpenGL initialization
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);					// 平滑着色

	glClearDepth(1.0f);							// 设置深度缓存
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glCullFace(GL_BACK);						// 只绘制正面
	glEnable(GL_CULL_FACE);

	//texture
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glEnable(GL_TEXTURE_2D);
# else
    
	glClearDepthf(1.0f);							// 设置深度缓存
	glEnable(GL_DEPTH_TEST);
# endif

	CZCheckGLError();

	/// load shader
    loadShaders();
    
	/// config scene
	if(1||!load(sceneFilename))
	{
		scene.eyePosition = CZPoint3D(0, 0, -200);
		scene.light.position = CZPoint3D(0, 0, -120);
		scene.light.intensity = CZPoint3D(1, 1, 1);
		scene.ambientLight.intensity = CZPoint3D(0.2,0.2,0.2);
		scene.directionalLight.intensity = CZPoint3D(1,1,1);
		scene.directionalLight.direction = CZPoint3D(0,-5,10);
        
       // scene.light.position = CZPoint3D(-105.351,-86.679,-133.965);
        scene.light.intensity = CZPoint3D(1, 1, 1);
        scene.ambientLight.intensity = CZPoint3D(0.2,0.2,0.2);
        scene.directionalLight.intensity = CZPoint3D(1,1,1);
        scene.directionalLight.direction = CZPoint3D(-105.351,-86.679,133.965);
		scene.bgColor = CZColor(0.8f, 0.8f, 0.9f, 1.f);
		scene.mColor = CZColor(1.f, 1.f, 1.f, 1.f);
	}

	CZCheckGLError();

	return true;
}

bool Application3D::loadObjModel(const char* filename, bool quickLoad /* = true */)
{
	if(filename == NULL) 
	{
		LOG_ERROR("filename is NULL\n");
		return false;
	}

	if (pModel) delete pModel;
	pModel = new CZObjModel;

    bool success = false;
    string strFileName(filename);
    string tempFileName = strFileName + ".b";
    if(documentDirectory)
    {
        size_t splitLoc = tempFileName.find_last_of('/');
        size_t strLen = tempFileName.length();
        string name = tempFileName.substr(splitLoc+1,strLen-splitLoc-1);
        tempFileName = string(documentDirectory) + "/" + name;
    }
    
	if (!quickLoad || !pModel->loadBinary(tempFileName,filename))
	{
		success = pModel->load(strFileName);
		if(success && quickLoad)
            pModel->saveAsBinary(tempFileName);
	}

	reset();

	CZCheckGLError();

	return true;
}

bool Application3D::setRenderBufferSize(int w, int h)
{
	width = w;	height = h;

	glViewport(0,0,width,height);
#if !defined(__APPLE__)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)width/(GLfloat)height, 0.5f, 500.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

# endif
    
    projMat.SetPerspective(60.0,(GLfloat)width/(GLfloat)height, 0.5f, 500.0f);
    
	return true;
}

void Application3D::frame()
{
    CZCheckGLError();
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清理颜色缓冲区 和 深度缓冲区

#if !defined(__APPLE__)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(scene.eyePosition.x,scene.eyePosition.y,scene.eyePosition.z, 0,0,0,0,1,0);
#endif
    
	CZMat4 mvpMat,modelMat;
	modelMat = translateMat * scaleMat * rotateMat;
	mvpMat.SetLookAt(scene.eyePosition.x, scene.eyePosition.y, scene.eyePosition.z, 0, 0, 0, 0, 1, 0);
	mvpMat = projMat * mvpMat * modelMat;

	/// 绘制
	CZShader *pShader = getShader(kDirectionalLightShading);

	if (pShader == NULL)
	{
		LOG_ERROR("there's no shader designated\n");
		return;
	}
	CZCheckGLError();
	pShader->begin();
	CZCheckGLError();
	// common uniforms
	glUniformMatrix4fv(pShader->getUniformLocation("mvpMat"), 1, GL_FALSE, mvpMat);
	glUniformMatrix4fv(pShader->getUniformLocation("modelMat"), 1, GL_FALSE, modelMat.GetInverseTranspose());
	glUniform3f(pShader->getUniformLocation("ambientLight.intensities"),
		scene.ambientLight.intensity.x,
		scene.ambientLight.intensity.y,
		scene.ambientLight.intensity.z);
	CZCheckGLError();
	glUniform3f(pShader->getUniformLocation("directionalLight.direction"),
		scene.directionalLight.direction.x,
		scene.directionalLight.direction.y, 
		scene.directionalLight.direction.z);

	glUniform3f(pShader->getUniformLocation("directionalLight.intensities"),
		scene.directionalLight.intensity.x,
		scene.directionalLight.intensity.y, 
		scene.directionalLight.intensity.z);
	CZCheckGLError();

	if(pModel)	pModel->draw(pShader);
	CZCheckGLError();

	pShader->end();
#if USE_OPENGL
	glColor3f(1.0,0.0,0.0);
	glPushMatrix();
	glTranslatef(scene.light.position.x, scene.light.position.y, scene.light.position.z);
	glDisable(GL_TEXTURE_2D);
	glutSolidSphere(2, 100, 100);
	glPopMatrix();
#endif
}

void Application3D::reset()
{
	/// model matrix
	rotateMat.LoadIdentity();
	translateMat.LoadIdentity();
	scaleMat.LoadIdentity();
   // translateMat.SetTranslation(-205400, 0.000000, 445500);
    
	/// color
	modelColor = scene.mColor;
	glClearColor(scene.bgColor.r, scene.bgColor.g, scene.bgColor.b, scene.bgColor.a);
}

// document directory
//  /note : default as the same of model's location;
//          should be set in ios platform to utilize binary data
void Application3D::setDocDirectory(const char* docDir)
{
    if (docDir == NULL)
    {
        LOG_WARN("docDir is NULL\n");
        return;
    }
    
    delete documentDirectory;
    size_t len = strlen(docDir);
    documentDirectory = new char[len+1];
    strcpy(documentDirectory, docDir);
    documentDirectory[len] = '\0';
}

// control
void Application3D::rotate(float deltaX, float deltaY)
{
	CZMat4 tempMat;
	tempMat.SetRotationY(deltaX);
	rotateMat = tempMat * rotateMat;
	tempMat.SetRotationX(-deltaY);
	rotateMat = tempMat * rotateMat;
}
void Application3D::translate(float deltaX, float deltaY)
{
	CZMat4 tempMat;
	tempMat.SetTranslation(-deltaX, -deltaY, 0);
	translateMat = tempMat * translateMat;
}
void Application3D::scale(float s)
{
	CZMat4 tempMat;
	tempMat.SetScale(s);
	scaleMat = tempMat * scaleMat;
}

// custom config
void Application3D::setBackgroundColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}
void Application3D::setModelColor(float r, float g, float b, float a)
{
	modelColor = CZColor(r, g, b, a);
}

void Application3D::setLightPosition(float x, float y, float z)
{
	scene.light.position = CZPoint3D(x, y, z);
}

//////////////////////////////////////////////////////////////////////////
bool Application3D::loadShaders()
{
	//
	vector<string> attributes;
	attributes.push_back("vert");
	attributes.push_back("vertNormal");
	attributes.push_back("vertTexCoord");
	vector<string> uniforms;
	uniforms.push_back("mvpMat");
	uniforms.push_back("modelMat");
	uniforms.push_back("ambientLight.direction");
	uniforms.push_back("ambientLight.intensities");
	uniforms.push_back("directionalLight.direction");
	uniforms.push_back("directionalLight.intensities");
	uniforms.push_back("tex");
	uniforms.push_back("hasTex");
	uniforms.push_back("material.kd");
	uniforms.push_back("material.ka");

	CZShader *pShader = new CZShader("standard","directionalLight",attributes,uniforms);
	shaders.insert(make_pair(kDirectionalLightShading,pShader));
	
	CZCheckGLError();

	return true;
}

CZShader* Application3D::getShader(ShaderType type)
{
	ShaderMap::iterator itr = shaders.find(type);
	
	return itr != shaders.end() ?	itr->second : NULL;
}

void Application3D::parseLine(ifstream& ifs, const string& ele_id)
{
	if ("camera_position" == ele_id)
		parseEyePosition(ifs);

	else if ("pl" == ele_id)
		parsePointLight(ifs);

	else if ("dl" == ele_id)
		parseDirectionalLight(ifs);
	else if ("dl_direction" == ele_id)
		ifs >> scene.directionalLight.direction.x >>
		scene.directionalLight.direction.y >>
		scene.directionalLight.direction.z;
	else if ("dl_color" == ele_id)
		ifs >> scene.directionalLight.intensity.x >>
		scene.directionalLight.intensity.y >>
		scene.directionalLight.intensity.z;
	else if ("dl_intensity" == ele_id)
	{
		float intensity;
		ifs >> intensity;
		scene.directionalLight.intensity.x *= intensity;
		scene.directionalLight.intensity.y *= intensity;
		scene.directionalLight.intensity.z *= intensity;
	}

	else if ("al_color" == ele_id)
		ifs >> scene.ambientLight.intensity.x >>
		scene.ambientLight.intensity.y >>
		scene.ambientLight.intensity.z;
	else if ("al_intensity" == ele_id)
	{
		float intensity;
		ifs >> intensity;
		scene.ambientLight.intensity.x *= intensity;
		scene.ambientLight.intensity.y *= intensity;
		scene.ambientLight.intensity.z *= intensity;
	}

	else if ("background_color" == ele_id)
		parseBackgroundColor(ifs);
	else if ("render_color" == ele_id)
		parseMainColor(ifs);
	else
		skipLine(ifs);
}

void Application3D::parseEyePosition(ifstream& ifs)
{
	ifs >> scene.eyePosition.x
		>> scene.eyePosition.y
		>> scene.eyePosition.z;
}

void Application3D::parsePointLight(ifstream& ifs)
{
	float intensity;
	ifs >> scene.light.position.x
		>> scene.light.position.y
		>> scene.light.position.z
		>> scene.light.intensity.x
		>> scene.light.intensity.y
		>> scene.light.intensity.z
		>> intensity;
	scene.light.intensity.x *= intensity;
	scene.light.intensity.y *= intensity;
	scene.light.intensity.z *= intensity;
}

void Application3D::parseDirectionalLight(ifstream& ifs)
{
	float intensity;
	ifs >> scene.directionalLight.direction.x
		>> scene.directionalLight.direction.y
		>> scene.directionalLight.direction.z
		>> scene.directionalLight.intensity.x
		>> scene.directionalLight.intensity.y
		>> scene.directionalLight.intensity.z
		>> intensity;
	scene.directionalLight.intensity.x *= intensity;
	scene.directionalLight.intensity.y *= intensity;
	scene.directionalLight.intensity.z *= intensity;
}

void Application3D::parseBackgroundColor(ifstream& ifs)
{
	ifs >> scene.bgColor.r
		>> scene.bgColor.g
		>> scene.bgColor.b
		>> scene.bgColor.a;
}

void Application3D::parseMainColor(ifstream& ifs)
{
	ifs >> scene.mColor.r
		>> scene.mColor.g
		>> scene.mColor.b
		>> scene.mColor.a;
}
