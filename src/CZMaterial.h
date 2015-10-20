#ifndef _CZMATERIAL_H_
#define _CZMATERIAL_H_

#include <string>
#include <fstream>

class CZMaterial
{
public:
	CZMaterial();

	bool load(const std::string &filename);
	void use() const;

private:
	bool loadTexture(const std::string &filename);
	void parseLine(std::ifstream &ifs,std::string &elementId);
	bool skipCommentLine(std::ifstream &ifs);
	void skipLine(std::ifstream &ifs);

public:
	float	Ns;		//	shininess
	float	Ka[4];	//	ambient color
	float	Kd[4];	//  diffuse color
	float	Ks[4];	//	specular color

private:
	std::string	 dirPath;
	unsigned int texId;
};

#endif