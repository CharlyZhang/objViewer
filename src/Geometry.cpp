#include "Geometry.h"

#include "gl/glut.h"

// Static member
GLuint CGeometry::ATTRIB_POS_DEFAULT = 0;
GLuint CGeometry::ATTRIB_NORM_DEFAULT = 2;
GLuint CGeometry::ATTRIB_TEX_COORD_DEFAULT = 8;

CGeometry::CGeometry()
: m_minVert(numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max())
, m_maxVert(-numeric_limits<float>::max(), -numeric_limits<float>::max(), -numeric_limits<float>::max())
{
	
}

CGeometry::~CGeometry()
{
	glDeleteBuffers(1, &m_vao);
	glDeleteBuffers(1, &m_vboPos);
	glDeleteBuffers(1, &m_vboNorm);
	glDeleteBuffers(1, &m_vboTexCoord);
}

//@side-effect openGL绑定了另一个VAO对象
void CGeometry::draw() const
{
	pMaterial->use();

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, m_numVert);
}

void CGeometry::unpackTo(vector<CZVector3D<float>> *pPosVector, vector<CZVector3D<float>> *pNormVector, vector<CZVector3D<float>> *pTexCoordVector,
	const vector<CZVector3D<float>> &posRawVector, const vector<CZVector3D<float>> &normRawVector, const vector<CZVector3D<float>> &texCoordRawVector) const{
	for (vector<CZFace>::const_iterator itFace = m_faceVector.begin(); itFace != m_faceVector.end(); ++itFace)
	{
		for (unsigned i = 0; i < (*itFace).v.size(); ++i) {
			/*相当于立即模式下，下列代码：*/
			//glNormal3fv(m_normRawVector[(*itFace).vn[i]].getVertex3fv());
			//glTexCoord2fv(m_texRawVector[(*itFace).vt[i]].getVertex3fv());
			//glVertex3fv(m_vertRawVector[(*itFace).v[i]].getVertex3fv());

			pPosVector->push_back(posRawVector[(*itFace).v[i]]);
			pNormVector->push_back(normRawVector[(*itFace).vn[i]]);
			pTexCoordVector->push_back(texCoordRawVector[(*itFace).vt[i]]);
		}
	}
}

//@side-effect openGL分配了一个VAO、若干VBO，并设置它们的状态
void CGeometry::unpack(const vector<CZVector3D<float>> &posRawVector, const vector<CZVector3D<float>> &normRawVector, const vector<CZVector3D<float>> &texCoordRawVector){
	vector<CZVector3D<float>> posVector;
	vector<CZVector3D<float>> normVector;
	vector<CZVector3D<float>> texCoordVector;
	unpackTo(&posVector, &normVector, &texCoordVector,
		posRawVector, normRawVector, texCoordRawVector);
	m_numVert = posVector.size();

	/*模型对象（vao）*/
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	/*顶点位置分量对象（vbo）*/
	glGenBuffers(1, &m_vboPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboPos);
	glBufferData(GL_ARRAY_BUFFER, posVector.size() * 3 * sizeof(GLfloat), posVector.data(), GL_STATIC_DRAW);

	//法向量分量对象（vbo）
	glGenBuffers(1, &m_vboNorm);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNorm);
	glBufferData(GL_ARRAY_BUFFER, normVector.size() * 3 * sizeof(GLfloat), normVector.data(), GL_STATIC_DRAW);

	//纹理坐标分量对象（vbo）
	glGenBuffers(1, &m_vboTexCoord);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboTexCoord);
	glBufferData(GL_ARRAY_BUFFER, texCoordVector.size() * 3 * sizeof(GLfloat), texCoordVector.data(), GL_STATIC_DRAW);

	bind(ATTRIB_POS_DEFAULT, ATTRIB_NORM_DEFAULT, ATTRIB_TEX_COORD_DEFAULT);
}

void CGeometry::bind(GLuint attribPos, GLuint attribNorm, GLuint attribTexCoord)
{
	glBindVertexArray(m_vao);

	glDisableVertexAttribArray(m_lastAttribPos);
	glDisableVertexAttribArray(m_vboNorm);
	glDisableVertexAttribArray(m_vboTexCoord);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboPos);
	glEnableVertexAttribArray(attribPos);
	glVertexAttribPointer(attribPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	m_lastAttribPos = attribPos;

	glBindBuffer(GL_ARRAY_BUFFER, m_vboNorm);
	glEnableVertexAttribArray(attribNorm);
	glVertexAttribPointer(attribNorm, 3, GL_FLOAT, GL_FALSE, 0, 0);
	m_lastAttribNorm = attribNorm;

	glBindBuffer(GL_ARRAY_BUFFER, m_vboTexCoord);
	glEnableVertexAttribArray(attribTexCoord);
	glVertexAttribPointer(attribTexCoord, 3, GL_FLOAT, GL_FALSE, 0, 0);
	m_lastAttribTexCoord = attribTexCoord;
}