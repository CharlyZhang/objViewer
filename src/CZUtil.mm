#include "CZDefine.h"
#include "CZBasic.h"
#include <string>

#if defined(_WIN32)
#   include "FreeImage.h"
#elif defined(__APPLE__)
#   import <Foundation/Foundation.h>
#   import <UIKit/UIKit.h>
#endif

using namespace std;

void CZCheckGLError_(const char *file, int line)
{
	int    retCode = 0;
	GLenum glErr = glGetError();

	while (glErr != GL_NO_ERROR) 
	{

#if defined USE_OPENGL
		const GLubyte* sError = gluErrorString(glErr);

		if (sError)
			printf("GL Error #%d (%s) in File %s at line: %d\n",glErr,gluErrorString(glErr),file,line);
		else
			printf("GL Error #%d (no message available) in File %s at line: %d\n",glErr,file,line);

#elif defined USE_OPENGL_ES
		switch (glErr) {
		case GL_INVALID_ENUM:
			printf("(%s): %d - GL Error: Enum argument is out of range\n",file,line);
			break;
		case GL_INVALID_VALUE:
			printf("(%s): %d - GL Error: Numeric value is out of range\n",file,line);
			break;
		case GL_INVALID_OPERATION:
			printf("(%s): %d - GL Error: Operation illegal in current state\n",file,line);
			break;
			//        case GL_STACK_OVERFLOW:
			//            NSLog(@"GL Error: Command would cause a stack overflow");
			//            break;
			//        case GL_STACK_UNDERFLOW:
			//            NSLog(@"GL Error: Command would cause a stack underflow");
			//            break;
		case GL_OUT_OF_MEMORY:
			printf("(%s): %d - GL Error: Not enough memory to execute command\n",file,line);
			break;
		case GL_NO_ERROR:
			if (1) {
				printf("No GL Error\n");
			}
			break;
		default:
			printf("(%s): %d - Unknown GL Error\n",file,line);
			break;
		}
#endif

		retCode = 1;
		glErr = glGetError();
	}
	//return retCode;
};

CZImage *CZLoadTexture(const string &filename)
{
#if !defined(__APPLE__)
    //image format
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    //pointer to the image, once loaded
    FIBITMAP *dib(0);
    //pointer to the image data
    BYTE* bits(0);
    //image width and height
    unsigned int width(0), height(0);
    //OpenGL's image ID to map to
    GLuint gl_texId;
    
    //check the file signature and deduce its format
    fif = FreeImage_GetFileType(filename.c_str(), 0);
    //if still unknown, try to guess the file format from the file extension
    if (fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(filename.c_str());
    //if still unkown, return failure
    if (fif == FIF_UNKNOWN)
        return false;
    
    //check that the plugin has reading capabilities and load the file
    if (FreeImage_FIFSupportsReading(fif))
        dib = FreeImage_Load(fif, filename.c_str());
    //if the image failed to load, return failure
    if (!dib)
        return false;
    
    //retrieve the image data
    bits = FreeImage_GetBits(dib);
    //get the image width and height
    width = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);
    //if this somehow one of these failed (they shouldn't), return failure
    if ((bits == 0) || (width == 0) || (height == 0))
        return false;
    
    unsigned int bpp = FreeImage_GetBPP(dib);
    LOG_DEBUG("bpp is %u\n",bpp);
    FREE_IMAGE_TYPE type = FreeImage_GetImageType(dib);
    FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(dib);
    
    // TO DO: inverse pixel data sequence manually
    GLenum texFormat;	GLint internalFormat;	GLint components;
    switch (colorType)
    {
        case FIC_RGB:
            components = 3;
            texFormat = GL_RGB;
            internalFormat = GL_RGB;
            break;
        case FIC_RGBALPHA:
            components = 4;
            texFormat = GL_BGRA_EXT;
            internalFormat = GL_BGRA_EXT;
            break;
        default:
            components = 3;
            texFormat = GL_RGB;
            internalFormat = GL_RGB;
            LOG_WARN("the color type has not been considered\n");
            break;
    }
    
    //generate an OpenGL texture ID for this texture
    glGenTextures(1, &gl_texId);
    //store the texture ID mapping
    texId = gl_texId;
    //bind to the new texture ID
    glBindTexture(GL_TEXTURE_2D, gl_texId);
    //store the texture data for OpenGL use
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height,
                 0, texFormat, GL_UNSIGNED_BYTE, bits);
    
    //	gluBuild2DMipmaps(GL_TEXTURE_2D, components, width, height, texFormat, GL_UNSIGNED_BYTE, bits);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //Free FreeImage's copy of the data
    FreeImage_Unload(dib);
    
    // TO DO: to load bmp with bpp=32
    
#else
    UIImage *image = [UIImage imageWithContentsOfFile:[NSString stringWithCString:filename.c_str() encoding:NSUTF8StringEncoding]];
    
    CGImageRef img = image.CGImage;
    
    //数据源提供者
    CGDataProviderRef inProvider = CGImageGetDataProvider(img);
    // provider’s data.
    CFDataRef inBitmapData = CGDataProviderCopyData(inProvider);
    
    const UInt8 *data = CFDataGetBytePtr(inBitmapData);
    
    //宽，高，data
    size_t width= CGImageGetWidth(img);
    size_t height = CGImageGetHeight(img);
    
    CZImage *retImage = new CZImage;
    retImage->width = (int)width;
    retImage->height = (int)height;
    retImage->data = (unsigned char*)malloc(width*height*4);
    unsigned char *src = (UInt8*)&data[(height-1)*width*4];
    UInt8 *dst = retImage->data;
    for (int i=0; i<height; i++)
    {
        memcpy(dst,src,width*4);
        dst += (width*4);
        src -= (width*4);
    }
    
    CFRelease(inBitmapData);
    
    return retImage;
#endif
}