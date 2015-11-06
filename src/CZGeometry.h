#ifndef __GEMORETRY_HPP__
#define __GEMORETRY_HPP__

#include "CZVector.h"
#include <vector>
#include <string>

/// CZFace
class CZFace
{
public:
	CZFace()
	{
		v.reserve(3);
		vt.reserve(3);
		vn.reserve(3);
	}

	~CZFace() {	v.clear(); vt.clear(); vn.clear();}

	void addVertTexNorm(int vi, int ti, int ni)
	{
		v.push_back(vi);
		vt.push_back(ti);
		vn.push_back(ni);
	}

	std::vector<int> v;		///<	vertex indices
	std::vector<int> vt;	///<	texture indices
	std::vector<int> vn;	///<	normal indices
};

/// CZGeometry
class CZGeometry
{
public:

	CZGeometry();
	~CZGeometry();

	void addFace(const CZFace& face)
	{
		if (face.vn[0] != -1)	hasNormal = true;
		if (face.vt[0] != -1)	hasTexCoord = true;

		faces.push_back(face);
	}
	/// unpack the raw data 
	void unpack(const std::vector<CZVector3D<float>> &posRawVector,	\
			const std::vector<CZVector3D<float>> &normRawVector,	\
			const std::vector<CZVector3D<float>> &texCoordRawVector);


	// render data
	std::vector<CZVector3D<float>> positions;
	std::vector<CZVector3D<float>> normals;
	std::vector<CZVector3D<float>> texcoords;

	std::vector<CZFace> faces;			///< 面片
	CZVector3D<float> aabbMin,aabbMax;	///< aabb 包围盒
	std::string materialName;			///< 纹理名称

	bool hasNormal;
	bool hasTexCoord;

private:
	/// generate face normals 
	void generateFaceNorm();	
	/// update aabb
	void updateAABB(CZVector3D<float> p);
};

#endif
