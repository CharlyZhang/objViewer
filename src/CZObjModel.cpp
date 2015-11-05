#include "CZObjModel.h"
#include "CZMaterial.h"
#include "CZLog.h"
#include "CZDefine.h"

using namespace std;

CZObjModel::CZObjModel()
{
	pCurGeometry = NULL;
}
CZObjModel::~CZObjModel()
{
	// geometry
	for (vector<CZGeometry*>::iterator itr = geometries.begin(); itr != geometries.end(); itr++)
	{
		delete *itr;
	}
	geometries.clear();

	clearRawData();
}

bool CZObjModel::load(const string& path)
{
	LOG_INFO("Parsing %s ...\n", path.c_str());

	if(CZObjFileParser::load(path) == false) return false;

	/// unpack each geometry
	for (vector<CZGeometry*>::iterator itr = geometries.begin(); itr != geometries.end(); itr++)
	{
		CZGeometry *pGeometry = (*itr);
		pGeometry->unpack(m_vertRawVector, m_normRawVector, m_texRawVector);
	}

	/// load material lib
	materialLib.load(m_dir + "/" + mtlLibName);

	clearRawData();

	return true;
}

bool CZObjModel::saveAsBinary(const std::string& path)
{
	FILE *fp = fopen(path.c_str(), "wb");

	if (fp == NULL)
	{
		LOG_ERROR("file open failed\n");
		return false;
	}
	// material lib name
	unsigned char mtlLibNameLen = mtlLibName.size();
	fwrite((char*)&mtlLibNameLen, sizeof(unsigned char), 1, fp);
	fwrite((char*)mtlLibName.c_str(), sizeof(char), mtlLibNameLen, fp);

	// geometry
	unsigned short count = geometries.size();
	fwrite((char*)(&count), sizeof(count), 1, fp);

	for (vector<CZGeometry*>::iterator itr = geometries.begin(); itr != geometries.end(); itr++)
	{
		CZGeometry *p = *itr;
		long numVert = p->positions.size();
		// material name
		unsigned char mtlNameLen = p->materialName.size();
		fwrite(&mtlNameLen, sizeof(unsigned char), 1, fp);
		fwrite(p->materialName.c_str(), sizeof(char), mtlNameLen, fp);

		// data
		fwrite(&(numVert), sizeof(numVert), 1, fp);
		fwrite(p->positions.data(), sizeof(CZVector3D<float>), numVert, fp);
		fwrite(p->normals.data(), sizeof(CZVector3D<float>), numVert, fp);

		unsigned char c_hasTexCoords = p->hasTexCoord ? 1 : 0;
		fwrite(&c_hasTexCoords, sizeof(bool), 1, fp);
		if (p->hasTexCoord)	
			fwrite((char*)p->texcoords.data(), sizeof(CZVector3D<float>), numVert, fp);
	}

	fclose(fp);

	return true;
}

bool CZObjModel::loadBinary(const std::string& path)
{
	m_dir = path.substr(0, path.find_last_of('/'));

	FILE *fp = fopen(path.c_str(), "rb");

	if (fp == NULL)
	{
		LOG_ERROR("file open failed\n");
		return false;
	}
	
	unsigned char mtlLibNameLen;
	fread((char*)&mtlLibNameLen, sizeof(unsigned char), 1, fp);
	mtlLibName.resize(mtlLibNameLen);
	fread((char*)mtlLibName.c_str(), sizeof(char), mtlLibNameLen, fp);


	// geometry
	unsigned short count;
	fread((char*)(&count), sizeof(count), 1, fp);

	for (int iGeo = 0; iGeo < count; iGeo++)
	{
		CZGeometry *pNewGeometry = new CZGeometry();
		
		// material name
		unsigned char mtlLibNameLen;

		fread(&mtlLibNameLen, sizeof(unsigned char), 1, fp);
		pNewGeometry->materialName.resize(mtlLibNameLen);
		fread((char*)pNewGeometry->materialName.c_str(), sizeof(char), mtlLibNameLen, fp);

		// data
		long numVert;
		fread(&numVert, sizeof(numVert), 1, fp);

		pNewGeometry->positions.resize(numVert);
		pNewGeometry->normals.resize(numVert);
		fread(pNewGeometry->positions.data(), sizeof(CZVector3D<float>), numVert, fp);
		fread(pNewGeometry->normals.data(), sizeof(CZVector3D<float>), numVert, fp);

		unsigned char c_hasTexCoords;
		fread(&c_hasTexCoords, sizeof(unsigned char), 1, fp);
		if (c_hasTexCoords == 1)
		{
			pNewGeometry->hasTexCoord = true;
			pNewGeometry->texcoords.resize(numVert);
			fread((char*)pNewGeometry->texcoords.data(), sizeof(CZVector3D<float>), numVert, fp);
		}
		else
			pNewGeometry->hasTexCoord = false;

		geometries.push_back(pNewGeometry);
	}

	fclose(fp);

	materialLib.load(m_dir + "/" + mtlLibName);

	return true;
}

void CZObjModel::draw(CZShader* pShader)
{
	if(pShader == NULL) 
	{
		LOG_ERROR("pShader is NULL!\n");
		return;
	}

	for (vector<CZGeometry*>::iterator itr = geometries.begin(); itr != geometries.end(); itr++)
	{
		CZGeometry *pGeometry = *itr;
		CZMaterial *pMaterial = materialLib.get(pGeometry->materialName);

		auto ka = pMaterial->Ka;
		glUniform3f(pShader->getUniformLocation("ka"), ka[0], ka[1], ka[2]);
		auto kd = pMaterial->Kd;
		glUniform3f(pShader->getUniformLocation("kd"), kd[0], kd[1], kd[2]);

		int hasTex;
		if (pMaterial->use() && pGeometry->hasTexCoord)
				hasTex = 1;
		else	hasTex = 0;
		
		glUniform1i(pShader->getUniformLocation("hasTex"), hasTex);
		glUniform1i(pShader->getUniformLocation("tex"), 0);

		GLuint m_vao;
		long numVert = pGeometry->positions.size();

		GLuint m_vboPos;
		GLuint m_vboNorm;
		GLuint m_vboTexCoord;
		
		// vao
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		// vertex 
		glGenBuffers(1, &m_vboPos);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboPos);
		glBufferData(GL_ARRAY_BUFFER,pGeometry->positions.size() * 3 * sizeof(GLfloat), pGeometry->positions.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// normal
		glGenBuffers(1, &m_vboNorm);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboNorm);
		glBufferData(GL_ARRAY_BUFFER, pGeometry->normals.size() * 3 * sizeof(GLfloat), pGeometry->normals.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// texcoord
		if (pGeometry->hasTexCoord)
		{
			glGenBuffers(1, &m_vboTexCoord);
			glBindBuffer(GL_ARRAY_BUFFER, m_vboTexCoord);
			glBufferData(GL_ARRAY_BUFFER, pGeometry->texcoords.size() * 3 * sizeof(GLfloat), pGeometry->texcoords.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}


		glDrawArrays(GL_TRIANGLES, 0, numVert);

		glDeleteBuffers(1, &m_vao);
		glDeleteBuffers(1, &m_vboPos);
		glDeleteBuffers(1, &m_vboNorm);
		if (pGeometry->hasTexCoord) glDeleteBuffers(1, &m_vboTexCoord);

		glBindVertexArray(0);

	}
}

void CZObjModel::clearRawData()
{
	/*free memory£º
	 *£¨link£ºhttp://www.cppblog.com/lanshengsheng/archive/2013/03/04/198198.html£©
	 */

	m_vertRawVector.clear();
	m_vertRawVector.swap(vector<CZVector3D<float>>());

	m_texRawVector.clear();
	m_texRawVector.swap(vector<CZVector3D<float>>());

	m_normRawVector.clear();
	m_texRawVector.swap(vector<CZVector3D<float>>());
}

//////////////////////////////////////////////////////////////////////////

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

void CZObjModel::parseMaterialLib(std::ifstream &ifs)
{
	ifs >> mtlLibName;
	LOG_INFO("	mtllib %s \n", mtlLibName.c_str());
}

void CZObjModel::parseUseMaterial(std::ifstream &ifs)
{
	CZGeometry *pNewGeometry = new CZGeometry();
	pCurGeometry = pNewGeometry;
	ifs >> pNewGeometry->materialName;
	geometries.push_back(pNewGeometry);
	LOG_INFO("	usemtl %s\n",pNewGeometry->materialName.c_str());
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
	int data[3] = { -1, -1, -1 };
	int count;

	for (int i = 0; i < 3; i++){
		count = parseNumberElement(ifs, data);
		face.addVertTexNorm(data[0], data[1], data[2]);
	}
	skipLine(ifs);

	pCurGeometry->addFace(face);

	ifs.clear();
}
