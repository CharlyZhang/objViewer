#include "CZGeometry.h"
#include <cfloat>

using namespace std;

CZGeometry::CZGeometry(): aabbMin(CZVector3D<float>(FLT_MAX, FLT_MAX, FLT_MAX)), 
						  aabbMax(CZVector3D<float>(-FLT_MAX, -FLT_MAX, -FLT_MAX))
{
	hasNormal = false;
	hasTexCoord = false;
}

CZGeometry::~CZGeometry()
{
	faces.clear();
	positions.clear();
	normals.clear();
	texcoords.clear();
}

void CZGeometry::unpack(const vector<CZVector3D<float>> &posRawVector, \
						const vector<CZVector3D<float>> &normRawVector, \
						const vector<CZVector3D<float>> &texCoordRawVector)
{
	for (vector<CZFace>::const_iterator itr = faces.begin(); itr != faces.end(); ++itr)
	{
		for (unsigned i = 0; i < itr->v.size(); ++i) 
		{
			positions.push_back(posRawVector[itr->v[i]-1]);
			updateAABB(posRawVector[itr->v[i]-1]);
			if (hasNormal)	normals.push_back(normRawVector[itr->vn[i]-1]);
			if (hasTexCoord)	texcoords.push_back(texCoordRawVector[itr->vt[i]-1]);
			else				texcoords.push_back(CZVector3D<float>(0, 0, 0));
		}
	}

	if (!hasNormal)	generateFaceNorm();

	positions.shrink_to_fit();
	normals.shrink_to_fit();
	texcoords.shrink_to_fit();
}

//////////////////////////////////////////////////////////////////////////

void CZGeometry::generateFaceNorm()
{
	normals.clear();

	long vertNum = positions.size();
	for (long iVert = 0; iVert < vertNum; iVert += 3)
	{
		//一个面有3个顶点：v1、v2、v3
		const CZVector3D<float>& v1 = positions[iVert];
		const CZVector3D<float>& v2 = positions[iVert + 1];
		const CZVector3D<float>& v3 = positions[iVert + 2];

		//该面上有2个向量：向量v=v1->v2；向量w=v1->v3
		CZVector3D<float> v(v2.x-v1.x, v2.y-v1.y, v2.z-v1.z);
		CZVector3D<float> w(v3.x-v1.x, v3.y-v1.y, v3.z-v1.z);

		//向量v 向 向量w 作外积，结果即法向量vn
		CZVector3D<float> vn = v.cross(w);

		//对法向量vn单位化
		vn.normalize();

		//计算结果作为这3个顶点的法向量
		normals.push_back(vn);
		normals.push_back(vn);
		normals.push_back(vn);
	}
}

/// update aabb
void CZGeometry::updateAABB(CZVector3D<float> p)
{
	if(p.x > aabbMax.x)	aabbMax.x = p.x;
	if(p.x < aabbMin.x) aabbMin.x = p.x;

	if(p.y > aabbMax.y)	aabbMax.y = p.y;
	if(p.y < aabbMin.y) aabbMin.y = p.y;

	if(p.z > aabbMax.z)	aabbMax.z = p.z;
	if(p.z < aabbMin.z) aabbMin.z = p.z;
}