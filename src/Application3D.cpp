
#include "Application3D.h"
#include "CZDefine.h"
#include "CZBasic.h"
#include <vector>
#include <string>

#define DEFAULT_RENDER_SIZE 500				///< 默认渲染缓存大小

using namespace std;

Application3D::Application3D()
{
	width = height = DEFAULT_RENDER_SIZE;
	pScene = NULL;
	pShader = NULL;
}

Application3D::~Application3D()
{
	if(pScene) { delete pScene; pScene = NULL;}
	if(pShader) { delete pShader; pShader = NULL;}
}

bool Application3D::init()
{
	/// OpenGL initialization
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glShadeModel(GL_SMOOTH);					// 平滑着色

	glEnable(GL_NORMALIZE);
	glClearDepth(1.0f);							// 设置深度缓存
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(0.8f, 0.8f, 0.9f, 1.f);

	glCullFace(GL_BACK);						// 只绘制正面
	glEnable(GL_CULL_FACE);

	//texture
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glEnable(GL_TEXTURE_2D);

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
	pShader = new CZShader("shading","shading",attributes,uniforms);

	return true;
}

bool Application3D::setRenderBufferSize(int w, int h)
{
	width = w;	height = h;

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)width/(GLfloat)height, 0.5f, 500.0f);
	projMat.SetPerspective(60.0,(GLfloat)width/(GLfloat)height, 0.5f, 500.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return true;
}

bool Application3D::loadScene(const string &filename)
{
	// TO DO : load scene file
	if (pScene) delete pScene;
	
	pScene = new CZScene;

	pScene->eyePosition = CZPoint3D(0,0,-200);

	pScene->light.position = CZPoint3D(0,0,-120);
	pScene->light.intensity = CZPoint3D(1,1,1);

	pScene->model.load("../../data/plane/plane.obj");

	rotateMat.LoadIdentity();
	translateMat.LoadIdentity();

	return true;
}

void Application3D::frame()
{
	if(!pScene) 
	{
		LOG_WARN("scene has not been set up yet!\n");
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清理颜色缓冲区 和 深度缓冲区

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(pScene->eyePosition.x,pScene->eyePosition.y,pScene->eyePosition.z, 0,0,0,0,1,0);

	CZMat4 mvpMat,modelMat;
	modelMat = translateMat * rotateMat;
	mvpMat.SetLookAt(pScene->eyePosition.x,pScene->eyePosition.y,pScene->eyePosition.z, 0,0,0,0,1,0);
	mvpMat = projMat * mvpMat * modelMat;

	pShader->begin();
	CZCheckGLError();
	glUniformMatrix4fv(pShader->getUniformLocation("modelMat"),1,GL_FALSE,modelMat);
	glUniformMatrix4fv(pShader->getUniformLocation("mvpMat"),1,GL_FALSE,mvpMat);
	CZCheckGLError();
	//glUniform3fv(pShader->getUniformLocation("light.position"),3,&light.position[0]);
	//CZCheckGLError();
	glUniform3f(pShader->getUniformLocation("light.position"),pScene->light.position.x,pScene->light.position.y,pScene->light.position.z);
	CZCheckGLError();
	//glUniform3fv(pShader->getUniformLocation("light.intensities"),3,light.intensity);
	glUniform3f(pShader->getUniformLocation("light.intensities"),pScene->light.intensity.x,pScene->light.intensity.y,pScene->light.intensity.z);
	CZCheckGLError();
	//GLuint mVertexBufferObject;
	//// 装载顶点
	//glGenBuffers(1, &mVertexBufferObject);
	//glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STREAM_DRAW);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(float)*2,0);

	/// 绘制
	pScene->model.draw();
	CZCheckGLError();
	/*glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/// 消除
	glDeleteBuffers(1, &mVertexBufferObject);*/

	pShader->end();
	
	glColor3f(1.0,0.0,0.0);
	glPushMatrix();
	glTranslatef(pScene->light.position.x, pScene->light.position.y, pScene->light.position.z);
		glutSolidSphere(5,100,100);
	glPopMatrix();
}

// control
void Application3D::rotate(float deltaX, float deltaY)
{
	if(!pScene) 
	{
		LOG_WARN("scene has not been set up yet!\n");
		return;
	}
	CZMat4 tempMat;
	tempMat.SetRotationY(deltaX);
	rotateMat = tempMat * rotateMat;
	tempMat.SetRotationX(-deltaY);
	rotateMat = tempMat * rotateMat;
}

void Application3D::translate(float deltaX, float deltaY)
{
	if(!pScene) 
	{
		LOG_WARN("scene has not been set up yet!\n");
		return;
	}
	CZMat4 tempMat;
	tempMat.SetTranslation(-deltaX,-deltaY,0);
	translateMat = tempMat * translateMat;
}

// eye position
void Application3D::setEyePosition(float x, float y, float z)
{

	if(!pScene)
	{
		LOG_WARN("scene has not been set up yet!\n");
		return;
	}

	pScene->eyePosition = CZPoint3D(x,y,z);
}

void Application3D::setLightPosition(float x, float y, float z)
{
	if(!pScene) 
	{
		LOG_WARN("scene has not been set up yet!\n");
		return;
	}

	pScene->light.position = CZPoint3D(x,y,z);
}