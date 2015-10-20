
#include "CZObjModel.h"
#include "CZLog.h"
#include "glew.h"
#include "glut.h"

using namespace std;

bool CZObjModel::load(const std::string &filename)
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

	// check file size
	const int fileSize = ifs.tellg();
	if (0 == fileSize)
	{
		LOG_WARN("file(%s) size is zero\n", filename);
		return false;
	}

	LOG_INFO("loading - %s(size = %d bytes) ...\n",filename.c_str(),fileSize);
	ifs.seekg(0, ios::beg);
	
	// check file type
	checkFileType(ifs);

	// reset 
	normals.clear();
	texcoords.clear();
	vertex.clear();
	for (vector<CZFace*>::iterator itr = mesh.begin(); itr != mesh.end(); itr ++) delete *itr;
	mesh.clear();
	boxMin = CZVector3D<float>(FLT_MAX,FLT_MAX,FLT_MAX);
	boxMax = CZVector3D<float>(FLT_MIN,FLT_MIN,FLT_MIN);

	// parse model
	int percent = 10;				///<	progress indicator
	while( skipCommentLine(ifs) )
	{
		// show progress for files larger than one Mo
		if ((fileSize > 1024 * 1024) && (100 * ifs.tellg() / fileSize >= percent))
		{
			percent = 100 * ifs.tellg() / fileSize;
			percent = (percent / 10) * 10;
			LOG_INFO("  %d % done...\n",percent);
			percent += 10;
		}

		string elementId;
		if (!(ifs >> elementId))
			break;
		else
			parseLine(ifs, elementId);
	}

	ifs.close();

	unpack();

	return true;
}

bool CZObjModel::draw()
{
	material.use();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, m_numVert);
	return true;
}

bool CZObjModel::hasTexcoords() const
{
	return true;
}

bool CZObjModel::hasNormals() const
{
	return true;
}

//////////////////////////////////////////////////////////////////////////

void CZObjModel::checkFileType(ifstream &ifs)
{
	char buffer[1024];
	char elementId;
	ifs >> noskipws >> elementId;
	ifs.putback(elementId);

	// in case that no type description
	if(elementId != '#' && elementId != 'M')
	{
		fileType = kNormalObj;
		return;
	}

	ifs.getline(buffer, 1024);
	if(strstr(buffer,"# 3ds Max Wavefront OBJ Exporter") != NULL)
		fileType = kNavteqObj;
	else if(strstr(buffer,"Metasequoia Document") != NULL)
		fileType = kMqo;
	else 
		fileType = kNormalObj;
}

void CZObjModel::parseLine(ifstream &ifs, string &elementId)
{
	switch(fileType) 
	{
	case kNavteqObj:
	case kNormalObj:
		if(elementId == "v")			// v : vertex
			parseVertex(ifs);
		else if(elementId == "vn")		// vn : normal vertex
			parseVertexNormal(ifs);
		else if(elementId == "vt")		// vt : texture
			parseVertexTexcoord(ifs);
		else if(elementId == "f")		// f : Index
			parseFace(ifs);
		else if(elementId == "mtllib")	// .mtl File Name
			parseMaterialLib(ifs);
		else if (elementId == "usemtl") {
			string matName;
			ifs >> matName;
			cout << "	usemtl " << matName << endl;
			// TO DO: seperate model into different parts
		}
		else
			skipLine(ifs);
		break;
	}
}

void CZObjModel::parseVertexNormal(ifstream &ifs)
{
	CZVector3D<float> norm;
	ifs >> norm;
	normals.push_back(norm);
}

void CZObjModel::parseVertexTexcoord(ifstream &ifs)
{
	CZVector2D<float> texcoord;
	ifs >> texcoord;
	texcoords.push_back(texcoord);
}

void CZObjModel::parseVertex(ifstream &ifs)
{
	CZVector3D<float> vert;
	ifs >> vert;
	vertex.push_back(vert);

	if(vert.x > boxMax.x) boxMax.x = vert.x;	if(vert.x < boxMin.x) boxMin.x = vert.x;
	if(vert.y > boxMax.y) boxMax.y = vert.y;	if(vert.y < boxMin.y) boxMin.y = vert.y;
	if(vert.z > boxMax.z) boxMax.z = vert.z;	if(vert.z < boxMin.z) boxMin.z = vert.z;
}

void CZObjModel::parseFace(ifstream &ifs)
{
	//	face treatment
	//  Note: obviously this technique only works for convex polygons with ten verts or less.
	int vi[10];                               // vertex indices.
	int ni[10] = { -1, -1, -1, -1, };         // normal indices.
	int ti[10] = { -1, -1, -1, -1, };         // tex indices.
	int i = 0;
	for (char c; i < 10; ++i) {
		if (!hasTexcoords() && !hasNormals())
			ifs >> vi[i];
		else if (!hasTexcoords())
			ifs >> vi[i] >> c >> c >> ni[i];
		else if (!hasNormals())
			ifs >> vi[i] >> c >> ti[i];
		else
			ifs >> vi[i] >> c >> ti[i] >> c >> ni[i];

		if (!ifs.good())
			break;
	}
	//	Create the polygon face
	CZFace *pFace = new CZFace;
	for (int k = 0; k < i; ++k)
		pFace->addVertTexNormal(vi[k], ti[k], ni[k]);
	mesh.push_back(pFace);

	ifs.clear();
}

void CZObjModel::parseMaterialLib(ifstream &ifs)
{
	string mtlFilePath;		//mtl文件的相对路径（以obj文件所在目录为根目录）
	ifs >> mtlFilePath;
	LOG_INFO("	mtllib %s/%s\n", dirPath.c_str(), mtlFilePath.c_str());
	material.load(dirPath + "/" + mtlFilePath);
}

bool CZObjModel::skipCommentLine(ifstream &ifs)
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

void CZObjModel::skipLine(ifstream &ifs)
{
	char next;
	ifs >> noskipws;
	while ((ifs >> next) && ('\n' != next));
}

void CZObjModel::unpack()
{
	vector<CZVector3D<float>> vertVector;
	vector<CZVector3D<float>> normVector;
	vector<CZVector2D<float>> texVector;
	for (vector<CZFace*>::const_iterator itFace = mesh.begin(); itFace != mesh.end(); ++itFace)
	{
		for (unsigned i = 0; i < (*itFace)->vIndex.size(); ++i) {
			vertVector.push_back(vertex[(*itFace)->vIndex[i]-1]);
			normVector.push_back(normals[(*itFace)->nIndex[i]-1]);
			texVector.push_back(texcoords[(*itFace)->tIndex[i]-1]);
		}
	}

	m_numVert = vertVector.size();
	GLuint m_vboVert;
	GLuint m_vboNorm;
	GLuint m_vboTex;

	const GLuint VERT_POS = 0;
	const GLuint VERT_NORM = 1;
	const GLuint VERT_TEX = 2;


	/*模型对象（vao）*/
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/*顶点位置分量对象（vbo）*/
	glGenBuffers(1, &m_vboVert);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVert);
	glBufferData(GL_ARRAY_BUFFER, vertVector.size() * 3 * sizeof(GLfloat), vertVector.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(VERT_POS, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERT_POS);

	//法向量分量对象（vbo）
	glGenBuffers(1, &m_vboNorm);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNorm);
	glBufferData(GL_ARRAY_BUFFER, normVector.size() * 3 * sizeof(GLfloat), normVector.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(VERT_NORM, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERT_NORM);

	//纹理坐标分量对象（vbo）
	glGenBuffers(1, &m_vboTex);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboTex);
	glBufferData(GL_ARRAY_BUFFER, texVector.size() * 2 * sizeof(GLfloat), texVector.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(VERT_TEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERT_TEX);

	//vertVector.clear();
	//texVector.clear();
	//normVector.clear();
}