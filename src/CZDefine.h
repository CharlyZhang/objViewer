
#ifndef _CZDEFINE_H_
#define _CZDEFINE_H_

//
//#include "CZFbo.h"
//#include "CZShader.h"
//#include "CZTexture.h"

#define GLSL_DIR "../../src/glsl/"

//////////////////////////////////////////////////////////////////////////
//	OpenGL definition
//////////////////////////////////////////////////////////////////////////
#define USE_OPENGL 1

#if USE_OPENGL
/// include
 //#include <gl/GL.h>
 //#include <gl/GLU.h>
# include "glew.h"
# include "glut.h"
# include "CZLog.h"

/// type

/// functions
# define GL_GEN_VERTEXARRAY(n,arr)	glGenVertexArrays(n, arr)
# define GL_BIND_VERTEXARRAY(id)	glBindVertexArray(id)
# define GL_DEL_VERTEXARRAY(n,arr)	glDeleteVertexArrays(n,arr)
# define GL_DRAW_BUF(arr)           glDrawBuffer(arr)
# define GL_PUSH_ATTR(arr)          glPushAttrib(arr)
# define GL_POP_ATTR()              glPopAttrib()

#elif USE_OPENGL_ES
/// include
# include <OpenGLES/EAGL.h>
# include <OpenGLES/ES2/gl.h>
# include <OpenGLES/ES2/glext.h>

/// type
# define GL_RGB8					GL_RGB8_OES
# define GL_RGBA8					GL_RGBA8_OES

/// functions
# define GL_GEN_VERTEXARRAY(n,arr)	glGenVertexArraysOES(n, arr)
# define GL_BIND_VERTEXARRAY(id)	glBindVertexArrayOES(id)
# define GL_DEL_VERTEXARRAY(n,arr)	glDeleteVertexArraysOES(n,arr)
# define GL_DRAW_BUF(arr)
# define GL_PUSH_ATTR(arr)
# define GL_POP_ATTR()
#endif



extern void CZCheckGLError_(const char *file, int line);

#ifdef _DEBUG
#define CZCheckGLError()	CZCheckGLError_(__FILE__, __LINE__)
#else
#define CZCheckGLError()
#endif

#endif