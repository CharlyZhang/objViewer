#include "CZDefine.h"
#include "CZLog.h"

void CZCheckGLError_(const char *file, int line)
{
	int    retCode = 0;
	GLenum glErr = glGetError();

	while (glErr != GL_NO_ERROR) 
	{

#if USE_OPENGL
		const GLubyte* sError = gluErrorString(glErr);

		if (sError)
			LOG_INFO("GL Error #%d (%s) in File %s at line: %d\n",glErr,gluErrorString(glErr),file,line);
		else
			LOG_INFO("GL Error #%d (no message available) in File %s at line: %d\n",glErr,file,line);

#elif USE_OPENGL_ES
		switch (glErr) {
		case GL_INVALID_ENUM:
			LOG_ERROR("GL Error: Enum argument is out of range\n");
			break;
		case GL_INVALID_VALUE:
			LOG_ERROR("GL Error: Numeric value is out of range\n");
			break;
		case GL_INVALID_OPERATION:
			LOG_ERROR("GL Error: Operation illegal in current state\n");
			break;
			//        case GL_STACK_OVERFLOW:
			//            NSLog(@"GL Error: Command would cause a stack overflow");
			//            break;
			//        case GL_STACK_UNDERFLOW:
			//            NSLog(@"GL Error: Command would cause a stack underflow");
			//            break;
		case GL_OUT_OF_MEMORY:
			LOG_ERROR("GL Error: Not enough memory to execute command\n");
			break;
		case GL_NO_ERROR:
			if (1) {
				LOG_ERROR("No GL Error\n");
			}
			break;
		default:
			LOG_ERROR("Unknown GL Error\n");
			break;
		}
#endif

		retCode = 1;
		glErr = glGetError();
	}
	//return retCode;
};