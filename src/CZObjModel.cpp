#include "CZObjModel.h"

#include <string>
#include <fstream>
#include "Geometry.h"
#include "CZMaterial.h"
#include "CZLog.h"

using namespace std;

const int CZObjModel::FIRST_INDEX = 1;

CZObjModel::CZObjModel()
{
	//	Pushing some dummy front values.
	//	Note: this is uggly, find a better way.
	m_vertRawVector.push_back(CZVector3D<float>(0, 0, 0));
	m_texRawVector.push_back(CZVector3D<float>(0, 0, 0));
	m_normRawVector.push_back(CZVector3D<float>(0, 0, 0));

	m_pCurGeometry = NULL;
}

void CZObjModel::load(const string& path)
{
	LOG_INFO("Parsing .obj: %d", path);

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

void CZObjModel::parseLine(ifstream& ifs, const string& ele_id)
{
	if ("mtllib" == ele_id)
		parseMaterialLib(ifs);
	else if ("usemtl" == ele_id)
		parseUseMaterial(ifs);
	else if ("v" == ele_id)
		parseVertex(ifs);
	else if ("vt" == ele_id)
		parseVertexTexCoord(ifs);
	else if ("vn" == ele_id)
		parseVertexNormal(ifs);
	else if ("f" == ele_id)
		parseFace(ifs);
	else
		skipLine(ifs);
}

void CZObjModel::parseMaterialLib(std::ifstream &is)
{
	string mtlFilePath; //mtl文件的相对路径（以obj文件所在目录为根目录）
	is >> mtlFilePath;

	cout << "	mtllib " << (m_dir + "/" + mtlFilePath) << endl;
	m_materialLib.load(m_dir + "/" + mtlFilePath);
}

void CZObjModel::parseUseMaterial(std::ifstream &ifs)
{
	CGeometry *pNewGeometry = new CGeometry();
	m_pCurGeometry = pNewGeometry;
	ifs >> pNewGeometry->matrialName;
	m_geometries.push_back(pNewGeometry);
	cout << "	usemtl " << pNewGeometry->matrialName << endl;
}

void CZObjModel::parseVertex(std::ifstream &ifs)
{
	float x, y, z;
	ifs >> x >> y >> z;

	m_vertRawVector.push_back(CZVector3D<float>(x, y, z));
}

void CZObjModel::parseVertexNormal(std::ifstream &ifs)
{
	float x, y, z;
	ifs >> x >> y >> z;

	if (!ifs.good()) {                     // in case it is -1#IND00
		x = y = z = 0.0;
		ifs.clear();
		skipLine(ifs);
	}
	m_normRawVector.push_back(CZVector3D<float>(x, y, z));
}

void CZObjModel::parseVertexTexCoord(std::ifstream &ifs)
{
	float x, y, z;
	ifs >> x >> y >> z;
	ifs.clear();                           // is z (i.e. w) is not available, have to clear error flag.

	m_texRawVector.push_back(CZVector3D<float>(x, y, z));
}

void CZObjModel::parseFace(std::ifstream &ifs)
{
	CZFace face;
	int data[3] = {-1, -1, -1};
	int count;

	for (int i = 0; i < 3;i++){
		count = parseNumberElement(ifs, data);
		face.addVertTexNorm(data[0], data[1], data[2]);
	}
	skipLine(ifs);

	m_pCurGeometry->addFace(face);

	ifs.clear();
}
