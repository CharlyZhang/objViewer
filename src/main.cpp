#ifdef _WIN32
#include <windows.h>
#include <cstdlib>
#include <iostream>
#include "glew.h"
#include "glut.h"
#include "CZObjModel.h"
#include "Camera.h"
#include "CZShader.h"
#include "CZMat4.h"
#include "CZBasicStruct.h"
#include "CZDefine.h"

#endif

int winWidth = 800;
int winHeight = 600;

CZObjModel model;

CCamera camera;

CZLight light;

CZShader *pShader = NULL;

CZMat4 projMat;

using namespace std;

//初始化
void InitGL()
{
	glewInit();

	if (glewIsSupported("GL_VERSION_3_1"))
		cout << "Ready for OpenGL 3.1\n" ;
	else
	{
		cout << "OpenGL 3.1 not supported\n";
		//exit(1);
	}

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glutFullScreen();//全屏
	//glutSetCursor(GLUT_CURSOR_NONE);//隐藏鼠标指针
	//glutWarpPointer(0, 0);//设置鼠标指针相对于窗口左上角的位置

	glClearColor(0.8f, 0.8f, 0.9f, 1.f);//颜色格式：R G B A

	/*初始化顶点数组*/
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	//glPolygonMode(GL_FRONT, GL_FILL);



	//////////////////////////////////////////////////////////////////////////
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	//glShadeModel(GL_SMOOTH);					// 平滑着色

	//glEnable(GL_NORMALIZE);
	//glClearDepth(1.0f);							// 设置深度缓存
	//glEnable(GL_DEPTH_TEST);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//glCullFace(GL_BACK);						// 只绘制正面
	//glEnable(GL_CULL_FACE);

	//glEnable(GL_COLOR_MATERIAL);//允许根据顶点的颜色自动生成该顶点的光照材质
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	////light
	//glEnable(GL_LIGHT0);
	//float ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };//环境光
	//float diffuse[] = { 0.7f, 0.7f, 0.7f, 0.7f };//平行光
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	//glEnable(GL_LIGHTING);

	////texture
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	////glEnable(GL_TEXTURE_2D);

	glViewport(0,0,winWidth,winHeight);

	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();	
}

void Init()
{
	InitGL();

	CZCheckGLError();
	
	//model.load("../../data/box/box.obj");
	model.load("../../data/plane/plane.obj");
	camera.PositionCamera(0,0,-200, 0,0,0, 0,1,0);
	light.position[0] = 0; light.position[1] = 0; light.position[2] = -120;
	light.intensity[0] = 1; light.intensity[1] = 1; light.intensity[2] = 1;

	vector<string> attributes, uniforms;
	attributes.push_back("vert");
	attributes.push_back("vertNormal");
	attributes.push_back("vertTexCoord");
	uniforms.push_back("modelMat");
	uniforms.push_back("light.position");
	uniforms.push_back("light.intensities");
	pShader = new CZShader("vertex-shader","fragment-shader",attributes,uniforms);

	CZCheckGLError();
}

void Display()
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清理颜色缓冲区 和 深度缓冲区

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera.Look();
	CZMat4 mvMat;
	mvMat.LoadIdentity();
	CZCheckGLError();

	pShader->begin();
	CZCheckGLError();
	glUniformMatrix4fv(pShader->getUniformLocation("modelMat"),1,GL_FALSE,mvMat);
	CZCheckGLError();
	//glUniform3fv(pShader->getUniformLocation("light.position"),3,&light.position[0]);
	//CZCheckGLError();
	glUniform3f(pShader->getUniformLocation("light.position"),light.position[0],light.position[1],light.position[2]);
	CZCheckGLError();
	//glUniform3fv(pShader->getUniformLocation("light.intensities"),3,light.intensity);
	glUniform3f(pShader->getUniformLocation("light.intensities"),light.intensity[0],light.intensity[1],light.intensity[2]);
	CZCheckGLError();
	//GLuint mVertexBufferObject;
	//// 装载顶点
	//glGenBuffers(1, &mVertexBufferObject);
	//glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STREAM_DRAW);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(float)*2,0);

	/// 绘制
	model.draw();
	CZCheckGLError();
	/*glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/// 消除
	glDeleteBuffers(1, &mVertexBufferObject);*/

	pShader->end();
	
	//glutSolidSphere(10,50,50);

	glutSwapBuffers();
	glutReportErrors();
}
void Reshape(int w, int h)
{
	if (h == 0) h = 1;

	winWidth = w;	winHeight = h;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 0.5f, 500.0f);
	projMat.SetPerspective(60.0,(GLfloat)w/(GLfloat)h, 0.5f, 500.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	 
}
void key(unsigned char k, int x, int y) 
{
	k = (unsigned char)tolower(k);

 	switch(k) {
	case 27:
		exit(0);
		break;
	}
	glutPostRedisplay();
}
void specialKey(int key,int x,int y)
{
	switch(key) 
	{ 
	case GLUT_KEY_F1:
		break;
	} 
	glutPostRedisplay();
}

static bool mouseCtl = false;
static POINT mousePos;				
void Mouse(int iButton,int iState,int iXPos, int iYPos)
{
	if(iButton == GLUT_RIGHT_BUTTON && iState == GLUT_DOWN)
	{
		if(mouseCtl)
		{
			GetCursorPos(&mousePos);
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

		}
		else
		{
			SetCursorPos(mousePos.x,mousePos.y);
			glutSetCursor(GLUT_CURSOR_NONE);
		}
		mouseCtl=!mouseCtl;
	}
}

void idle() 
{
	if (mouseCtl)
	{
		camera.Update();
		glutPostRedisplay();
	}
	
}

void Menu(int value)
{
	switch(value)
	{
	case 1:
		break;
	}
}

void InitMenu()
{
	glutCreateMenu(Menu);
	glutAddMenuEntry("123",1);
	glutAddMenuEntry("adb",2);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);//把当前菜单注册到指定的鼠标键
}

int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(winWidth,winHeight);
	glutCreateWindow("objViewer");

	glutSetCursor(GLUT_CURSOR_NONE);

	Init();
	InitMenu();

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(key);
	glutSpecialFunc(specialKey); 
	glutMouseFunc(Mouse);
	glutIdleFunc(idle);
	glutMainLoop();

	delete pShader;
	return 0;
}