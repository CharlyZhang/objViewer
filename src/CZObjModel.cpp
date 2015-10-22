#include "CZObjModel.h"
#include "Geometry.h"
#include "CZMaterial.h"

#include <string>
#include <fstream>
using namespace std;

const int CZObjModel::FIRST_INDEX = 1;

CZObjModel::CZObjModel()
{
	//	Pushing some dummy front values.
	//	Note: this is uggly, find a better way.
	m_vertRawVector.push_back(CZVector3D<float>(0, 0, 0));
	m_texRawVector.push_back(CZVector3D<float>(0, 0, 0));
	m_normRawVector.push_back(CZVector3D<float>(0, 0, 0));
}

void CZObjModel::load(const string& path)
{
	cout << "Parsing .obj: " + path << endl;

	CObjFileParser::load(path);

	for (auto iterGeometry = m_geometries.begin(); iterGeometry != m_geometries.end(); iterGeometry++)
	{
		auto pGeometry = (*iterGeometry);
		pGeometry->pMaterial = m_materialLib.get(pGeometry->matrialName);
		pGeometry->unpack(m_vertRawVector, m_normRawVector, m_texRawVector);
	}

}

void CZObjModel::clearRaw()
{
	m_vertRawVector.clear();
	m_texRawVector.clear();
	m_normRawVector.clear();
}


void CZObjModel::draw()
{
	for (auto iterGeometry = m_geometries.begin(); iterGeometry != m_geometries.end(); iterGeometry++)
	{
		(*iterGeometry)->draw();
	}
}

void CZObjModel::processLine(istream& is, string ele_id)
{
	float x, y, z;

	if ("mtllib" == ele_id) {
		string mtlFilePath; //mtl文件的相对路径（以obj文件所在目录为根目录）
		is >> mtlFilePath;
		cout << "	mtllib " << (m_dir + "/" + mtlFilePath) << endl;
		m_materialLib.load(m_dir + "/" + mtlFilePath);
	}
	else if ("usemtl" == ele_id) {
		CGeometry *pNewGeometry = new CGeometry();
		m_pCurGeometry = pNewGeometry;
		is >> pNewGeometry->matrialName;
		m_geometries.push_back(pNewGeometry);
		cout << "	usemtl " << pNewGeometry->matrialName << endl;

	}
	else if ("v" == ele_id) {	//	vertex data
		is >> x >> y >> z;
		addVertex(CZVector3D<float>(x, y, z));
	}
	else if ("vt" == ele_id) {	// texture data
		is >> x >> y >> z;
		is.clear();                           // is z (i.e. w) is not available, have to clear error flag.
		addTextureCoord(CZVector3D<float>(x, y, z));
	}
	else if ("vn" == ele_id) {	// normal data
		is >> x >> y >> z;
		if (!is.good()) {                     // in case it is -1#IND00
			x = y = z = 0.0;
			is.clear();
			skipLine(is);
		}
		addNormal(CZVector3D<float>(x, y, z));
	}
	else if ("f" == ele_id) {	//	face data
		//	face treatment
		//  Note: obviously this technique only works for convex polygons with ten verts or less.
		int vi[10];                               // vertex indices.
		int ni[10] = { -1, -1, -1, -1, };         // normal indices.
		int ti[10] = { -1, -1, -1, -1, };         // tex indices.
		int i = 0;
		for (char c; i < 10; ++i) {
			if (!hasTextureCoords() && !hasNormals())
				is >> vi[i];
			else if (!hasTextureCoords())
				is >> vi[i] >> c >> c >> ni[i];
			else if (!hasNormals())
				is >> vi[i] >> c >> ti[i];
			else
				is >> vi[i] >> c >> ti[i] >> c >> ni[i];

			if (!is.good())
				break;
		}
		//	Create the polygon face
		CZFace face;
		for (int k = 0; k < i; ++k)
			face.addVertTexNorm(vi[k], ti[k], ni[k]);
		m_pCurGeometry->addFace(face);

		is.clear();
	}
	else
		skipLine(is);
}