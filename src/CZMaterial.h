#ifndef _CZMATERIAL_H_
#define _CZMATERIAL_H_

#include <string>
#include "CZDefine.h"

class CZMaterial
{
public:
	CZMaterial();

	bool loadTexture(const std::string &filename);
	bool use() const;

public:
	float	Ns;		///<	shininess
	float	Ka[4];	///<	ambient color
	float	Kd[4];	///<	diffuse color
	float	Ks[4];	///<	specular color

	bool hasTexture;
	unsigned int texId;

private:
	std::string	 dirPath;
};

#endif