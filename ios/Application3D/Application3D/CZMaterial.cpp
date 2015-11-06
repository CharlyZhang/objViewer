#include "CZMaterial.h"

#include "CZLog.h"
#include "CZDefine.h"

#if !defined(__APPLE__)
#   include "FreeImage.h"
#else
#   import <Foundation/Foundation.h>
#   import <UIKit/UIKit.h>
#endif

using namespace std;

CZMaterial::CZMaterial()
{
	Ns = 10;						//	shininess
	Ka[0] = Ka[1] = Ka[2] = 0.6f;	//	ambient color
	Ka[3] = 0;
	Kd[0] = Kd[1] = Kd[2] = 0.6f;	// diffuse color
	Kd[3] = 0;
	Ks[0] = Ks[1] = Ks[2] = Ks[3] = 0;	//	specular color

	hasTexture = false;
	texId = -1;
}

bool CZMaterial::loadTexture(const string &filename)
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
    
    //宽，高，data
    size_t width = CGImageGetWidth(img);
    size_t height = CGImageGetHeight(img);
    
    const UInt8 *data = CFDataGetBytePtr(inBitmapData);
    UInt8 *imageData = (UInt8*)malloc(width*height*4);
    UInt8 *src = (UInt8*)&data[(height-1)*width*4];
    UInt8 *dst = imageData;
    for (int i=0; i<height; i++) {
        memcpy(dst,src,width*4);
        dst += (width*4);
        src -= (width*4);
    }
    
    //generate an OpenGL texture ID for this texture
    glGenTextures(1, &texId);
    //bind to the new texture ID
    glBindTexture(GL_TEXTURE_2D, texId);
    //store the texture data for OpenGL use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    for (int i=0; i<10; i++) {
        printf("%d, %d, %d, %d \n",data[i*4+0],data[i*4+1],data[i*4+2],data[i*4+3]);
    }
    //	gluBuild2DMipmaps(GL_TEXTURE_2D, components, width, height, texFormat, GL_UNSIGNED_BYTE, bits);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


#endif
    hasTexture = true;
    return true;
}

bool CZMaterial::use() const
{
	if (texId != -1)
	{
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texId);
        //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#if !defined(__APPLE__)
        glEnable(GL_TEXTURE_2D);
#endif
		return true;
	}
	return false;
}
