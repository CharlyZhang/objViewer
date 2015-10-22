#ifdef _WIN32
#include <windows.h>
#include <cstdlib>
#include <iostream>
#include "glew.h"
#include "glut.h"
#include "Application3D.h"

#endif

int winWidth = 800;
int winHeight = 600;

Application3D app3d;

float lightX = 0;
float lightY = 0;
float lightZ = -120;

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
}

void Init()
{
	InitGL();

	app3d.init();
	string temp;
	app3d.loadScene(temp);
}

void Display()
{	
	app3d.frame();
	glutSwapBuffers();
	glutReportErrors();
}
void Reshape(int w, int h)
{
	if (h == 0) h = 1;

	winWidth = w;	winHeight = h;

	app3d.setRenderBufferSize(w,h);
	 
}
void key(unsigned char k, int x, int y) 
{
	k = (unsigned char)tolower(k);

 	switch(k) {
	case 27:
		exit(0);
		break;
	case 'w':
		lightY += 1.0;
		app3d.setLightPosition(lightX,lightY,lightZ);
		break;
	case 's':
		lightY -= 1.0;
		app3d.setLightPosition(lightX,lightY,lightZ);
		break;
	case 'a':
		lightX += 1.0;
		app3d.setLightPosition(lightX,lightY,lightZ);
		break;
	case 'd':
		lightX -= 1.0;
		app3d.setLightPosition(lightX,lightY,lightZ);
		break;
	case 'q':
		lightZ += 1.0;
		app3d.setLightPosition(lightX,lightY,lightZ);
		break;
	case 'e':
		lightZ -= 1.0;
		app3d.setLightPosition(lightX,lightY,lightZ);
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

static POINT lastMousePos;	
static bool isLeftButton = true;
void MouseClick(int iButton,int iState,int iXPos, int iYPos)
{
	if(iState == GLUT_DOWN)
	{
		lastMousePos.x = iXPos;
		lastMousePos.y = iYPos;

		if(iButton == GLUT_LEFT_BUTTON)			isLeftButton = true;
		else if(iButton == GLUT_RIGHT_BUTTON)	isLeftButton = false;
	}

	glutPostRedisplay();
}

void MouseMove(int x, int y)
{
	GLfloat offsetX = x - lastMousePos.x;
	GLfloat offsetY = y - lastMousePos.y;

	lastMousePos.x = x;
	lastMousePos.y = y;

	if(isLeftButton)	app3d.rotate((float)offsetX,(float)offsetY);
	else				app3d.translate((float)offsetX,(float)offsetY);
	glutPostRedisplay();
}

void idle() 
{
	
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

	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

	Init();
	InitMenu();

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(key);
	glutSpecialFunc(specialKey); 
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMove);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}


//////////////////////////////////////////////////////////////////////////
// camera operation
//////////////////////////////////////////////////////////////////////////