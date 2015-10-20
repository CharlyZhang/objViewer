
#include "CZMaterial.h"
#include "CZLog.h"
#include "FreeImage.h"
#include "glut.h"

using namespace std;

CZMaterial::CZMaterial()
{
	Ns = 10;						//	shininess
	Ka[0] = Ka[1] = Ka[2] = 0.6f;	//	ambient color
	Ka[3] = 0;
	Kd[0] = Kd[1] = Kd[2] = 0.6f;	// diffuse color
	Kd[3] = 0;
	Ks[0] = Ks[1] = Ks[2] = Ks[3] = 0;	//	specular color
}

bool CZMaterial::load(const string &filename)
{
	ifstream	ifs;				
		
	// open file
	ifs.open(filename.c_str(),ios::in | ios::ate);			
	if(ifs.fail()) 
	{
		LOG_ERROR("Unable to find or open the file: %s\n", filename.c_str());
		return false;
	}

	dirPath = filename.substr(0, filename.find_last_of('/'));

	// parse 
	ifs.seekg(0, ios::beg);
	while( skipCommentLine(ifs) )
	{
		string elementId;
		if (!(ifs >> elementId))
			break;
		else
			parseLine(ifs, elementId);
	}

	ifs.close();
	return true;
}

void CZMaterial::parseLine(ifstream &ifs,string &elementId)
{
	if ("newmtl" == elementId) {
		string matName;
		ifs >> matName;
		LOG_INFO("newmtl %s\n",matName.c_str());
	}
	else if ("Ns" == elementId) {	//shininess
		ifs >> Ns;
	}
	else if ("Ka" == elementId) {	//ambient color
		ifs >> Ka[0] >> Ka[1] >> Ka[2];
		ifs.clear();
	}
	else if ("Kd" == elementId) {	//diffuse color
		ifs >> Kd[0] >> Kd[1] >> Kd[2];
		ifs.clear();
	}
	else if ("Ks" == elementId) {	//specular color
		ifs >> Ks[0] >> Ks[1] >> Ks[2];
		ifs.clear();
	}
	else if ("map_Kd" == elementId) {
		string texImgPath;
		ifs >> texImgPath;//纹理图相对路径（以该mtl文件所在目录为根）

		texImgPath = dirPath + "/" + texImgPath;//转换到相对于程序根目录的相对路径
		bool success = loadTexture(texImgPath);
		// TO DO:
	}
	else
		skipLine(ifs);
}

bool CZMaterial::loadTexture(const string &filename)
{
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
		texFormat = GL_RGBA;
		internalFormat = GL_RGBA;
		break;
	default:
		components = 3;
		texFormat = GL_RGB;
		internalFormat = GL_RGBA;
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

	gluBuild2DMipmaps(GL_TEXTURE_2D, components, width, height, texFormat, GL_UNSIGNED_BYTE, bits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	// TO DO: to load bmp with bpp=32

	return true;
}

void CZMaterial::use() const
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ka);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Kd);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Ns);

	if (texId != 0)
	{
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);
	}
}

bool CZMaterial::skipCommentLine(ifstream &ifs)
{
	char next;
	while (ifs >> skipws >> next)
	{
		ifs.putback(next);
		if ('#' == next)
			skipLine(ifs);
		else
			return true;
	}

	return false;
}

void CZMaterial::skipLine(ifstream &ifs)
{
	char next;
	ifs >> noskipws;
	while ((ifs >> next) && ('\n' != next));
}
