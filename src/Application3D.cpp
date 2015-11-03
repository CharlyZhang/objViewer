
#include "Application3D.h"
#include "CZDefine.h"
#include <vector>
#include <string>

#define DEFAULT_RENDER_SIZE 500				///< 默认渲染缓存大小

using namespace std;

Application3D::Application3D()
{
	width = height = DEFAULT_RENDER_SIZE;
	pModel = NULL;
	pShader = NULL;
}

Application3D::~Application3D()
{
	if(pModel) { delete pModel; pModel = NULL;}
	if(pShader) { delete pShader; pShader = NULL;}
}

bool Application3D::init()
{
# if !defined(__APPLE__)
	/// OpenGL initialization
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glShadeModel(GL_SMOOTH);					// 平滑着色

	glEnable(GL_NORMALIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glCullFace(GL_BACK);						// 只绘制正面
	glEnable(GL_CULL_FACE);

	//texture
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glEnable(GL_TEXTURE_2D);
# endif
    
    glClearDepthf(1.0f);							// 设置深度缓存
    glEnable(GL_DEPTH_TEST);
    
	CZCheckGLError();

	/// load shader
	vector<string> attributes, uniforms;
	attributes.push_back("vert");
	attributes.push_back("vertNormal");
	attributes.push_back("vertTexCoord");
	uniforms.push_back("mvpMat");
	uniforms.push_back("modelMat");
	uniforms.push_back("light.position");
	uniforms.push_back("light.intensities");
    uniforms.push_back("hasTex");
	pShader = new CZShader("shading","shading",attributes,uniforms);

	/// config scene
	if(!loadScene())
	{
		scene.eyePosition = CZPoint3D(0,0,-200);
		scene.light.position = CZPoint3D(0,0,-120);
		scene.light.intensity = CZPoint3D(1,1,1);
		scene.bgColor = CZColor(0.8f, 0.8f, 0.9f, 1.f);
		scene.mColor = CZColor(1.f, 1.f, 1.f, 1.f);
	}

	return true;
}

bool Application3D::loadObjModel(const string &filename)
{
	if(pModel) delete pModel;
	pModel = new CZObjModel;
	pModel->load(filename);
	
	reset();

	return true;
}

bool Application3D::loadObjModel(const char *filename)
{
    if (!filename) {
        return false;
    }
    string f(filename);
    return loadObjModel(f);
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
	mvpMat.SetLookAt(scene.eyePosition.x,scene.eyePosition.y,scene.eyePosition.z, 0,0,0,0,1,0);
	mvpMat = projMat * mvpMat * modelMat;

	pShader->begin();
	CZCheckGLError();
	glUniformMatrix4fv(pShader->getUniformLocation("modelMat"),1,GL_FALSE,modelMat.GetInverseTranspose());
	glUniformMatrix4fv(pShader->getUniformLocation("mvpMat"),1,GL_FALSE,mvpMat);
	CZCheckGLError();
	//glUniform3fv(pShader->getUniformLocation("light.position"),3,&light.position[0]);
	//CZCheckGLError();
	glUniform3f(pShader->getUniformLocation("light.position"),scene.light.position.x,scene.light.position.y,scene.light.position.z);
	CZCheckGLError();
	//glUniform3fv(pShader->getUniformLocation("light.intensities"),3,light.intensity);
	glUniform3f(pShader->getUniformLocation("light.intensities"),scene.light.intensity.x,scene.light.intensity.y,scene.light.intensity.z);
	CZCheckGLError();
	//GLuint mVertexBufferObject;
	//// 装载顶点
	//glGenBuffers(1, &mVertexBufferObject);
	//glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STREAM_DRAW);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(float)*2,0);

	/// 绘制
	if(pModel)	pModel->draw(pShader);
	CZCheckGLError();
	/*glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/// 消除
	glDeleteBuffers(1, &mVertexBufferObject);*/

	pShader->end();
#if USE_OPENGL
	glColor3f(1.0,0.0,0.0);
	glPushMatrix();
	glTranslatef(scene.light.position.x, scene.light.position.y, scene.light.position.z);
	glDisable(GL_TEXTURE_2D);	
	glutSolidSphere(2,100,100);
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
	tempMat.SetTranslation(-deltaX,-deltaY,0);
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
	glClearColor(r,g,b,a);
}
void Application3D::setModelColor(float r, float g, float b, float a)
{
	modelColor = CZColor(r,g,b,a);
}

void Application3D::setLightPosition(float x, float y, float z)
{

	scene.light.position = CZPoint3D(x,y,z);
}

//////////////////////////////////////////////////////////////////////////

bool Application3D::loadScene()
{
	// TO DO : load scene file

	return false;
}