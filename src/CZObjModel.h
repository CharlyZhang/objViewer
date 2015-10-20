
#ifndef _CZOBJMODEL_H_
#define _CZOBJMODEL_H_

#include <string>
#include <vector>
#include <fstream>
#include "CZVector.h"
#include "CZMaterial.h"

/// file type
typedef enum _ObjFileType {
	kNavteqObj,
	kNormalObj,
	kMqo
} ObjFileType;

/// CZFace
class CZFace
{
public:
	CZFace(){	vIndex.reserve(3); tIndex.reserve(3); nIndex.reserve(3);};
	~CZFace(){	vIndex.clear();	tIndex.clear();	nIndex.clear();};
	void addVertTexNormal(int v,int t, int n)
	{
		vIndex.push_back(v);	tIndex.push_back(t);	nIndex.push_back(n);
	};

public:
	std::vector<int> vIndex,tIndex,nIndex;
};

/// CZObjModel
class CZObjModel
{
public:
	CZObjModel(){};
	~CZObjModel(){};

	/// 载入模型
	bool load(const std::string &filename);
	/// 绘制
	bool draw();
	/// 是否含有纹理坐标
	bool hasTexcoords() const;
	/// 是否含有法向量
	bool hasNormals() const;

private:
	void checkFileType(std::ifstream &ifs);
	void parseLine(std::ifstream &ifs,std::string &elementId);
	void parseVertexNormal(std::ifstream &ifs);
	void parseVertexTexcoord(std::ifstream &ifs);
	void parseVertex(std::ifstream &ifs);
	void parseFace(std::ifstream &ifs);
	void parseMaterialLib(std::ifstream &ifs);

	bool skipCommentLine(std::ifstream &ifs);
	void skipLine(std::ifstream &ifs);
	void unpack();

private:
	std::string	dirPath;				///< the diretory contains the model
	ObjFileType	fileType;				///< obj model type
	CZVector3D<float>	boxMin, boxMax;	///< bound box 
	CZMaterial material;

	std::vector < CZVector3D<float> >		vertex;
	std::vector	< CZVector2D<float> >		texcoords;
	std::vector	< CZVector3D<float> >		normals;
	std::vector < CZFace* >					mesh;

	unsigned int vao;
	size_t m_numVert;
};
#endif