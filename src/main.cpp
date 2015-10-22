#ifdef _WIN32
#include <windows.h>
#include <cstdlib>
#include <iostream>
#include "glew.h"
#include "glut.h"
#include "Camera.h"
#include "Application3D.h"

#endif

int winWidth = 800;
int winHeight = 600;

CCamera camera;

Application3D app3d;

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
	camera.PositionCamera(0,0,-200, 0,0,0, 0,1,0);
}

void Display()
{	
	app3d.setCamera(&camera);
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

	return 0;
}


//////////////////////////////////////////////////////////////////////////
// camera operation
//////////////////////////////////////////////////////////////////////////