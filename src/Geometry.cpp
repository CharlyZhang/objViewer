#include "Geometry.h"

#include "glut.h"

// Static member
GLuint CGeometry::ATTRIB_POS_DEFAULT = 0;
GLuint CGeometry::ATTRIB_NORM_DEFAULT = 1;
GLuint CGeometry::ATTRIB_TEX_COORD_DEFAULT = 2;

CGeometry::CGeometry()
: m_minVert(numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max())
, m_maxVert(-numeric_limits<float>::max(), -numeric_limits<float>::max(), -numeric_limits<float>::max())
{
	m_lastAttribPos = m_lastAttribNorm = m_lastAttribTexCoord = -1;

	m_hasNormals = false;
	m_hasTexCoords = false;
}

CGeometry::~CGeometry()
{
	glDeleteBuffers(1, &m_vao);
	glDeleteBuffers(1, &m_vboPos);
	glDeleteBuffers(1, &m_vboNorm);
	glDeleteBuffers(1, &m_vboTexCoord);
}

//@side-effect openGL������һ��VAO����
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
			/*�൱������ģʽ�£����д��룺*/
			//glNormal3fv(m_normRawVector[(*itFace).vn[i]].getVertex3fv());
			//glTexCoord2fv(m_texRawVector[(*itFace).vt[i]].getVertex3fv());
			//glVertex3fv(m_vertRawVector[(*itFace).v[i]].getVertex3fv());

			pPosVector->push_back(posRawVector[(*itFace).v[i]]);
			if (hasNormals())
				pNormVector->push_back(normRawVector[(*itFace).vn[i]]);

			if (hasTexCoords())
				pTexCoordVector->push_back(texCoordRawVector[(*itFace).vt[i]]);
			else
				pTexCoordVector->push_back(CZVector3D<float>(0, 0, 0));
		}
	}

	if (!hasNormals())
		generateFaceNorm(pNormVector, *pPosVector, pPosVector->size());
}

//@side-effect openGL������һ��VAO������VBO�����������ǵ�״̬
void CGeometry::unpack(const vector<CZVector3D<float>> &posRawVector, const vector<CZVector3D<float>> &normRawVector, const vector<CZVector3D<float>> &texCoordRawVector){
	vector<CZVector3D<float>> posVector;
	vector<CZVector3D<float>> normVector;
	vector<CZVector3D<float>> texCoordVector;
	unpackTo(&posVector, &normVector, &texCoordVector,
		posRawVector, normRawVector, texCoordRawVector);
	m_numVert = posVector.size();

	/*ģ�Ͷ���vao��*/
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	/*����λ�÷�������vbo��*/
	glGenBuffers(1, &m_vboPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboPos);
	glBufferData(GL_ARRAY_BUFFER, posVector.size() * 3 * sizeof(GLfloat), posVector.data(), GL_STATIC_DRAW);

	//��������������vbo��
	glGenBuffers(1, &m_vboNorm);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNorm);
	glBufferData(GL_ARRAY_BUFFER, normVector.size() * 3 * sizeof(GLfloat), normVector.data(), GL_STATIC_DRAW);

	//���������������vbo��
	if (hasTexCoords()){
		glGenBuffers(1, &m_vboTexCoord);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboTexCoord);
		glBufferData(GL_ARRAY_BUFFER, texCoordVector.size() * 3 * sizeof(GLfloat), texCoordVector.data(), GL_STATIC_DRAW);
	}

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

	if (hasTexCoords()){
		glBindBuffer(GL_ARRAY_BUFFER, m_vboTexCoord);
		glEnableVertexAttribArray(attribTexCoord);
		glVertexAttribPointer(attribTexCoord, 3, GL_FLOAT, GL_FALSE, 0, 0);
		m_lastAttribTexCoord = attribTexCoord;
	}
}

void CGeometry::generateFaceNorm(vector<CZVector3D<float>> *pNormVector, const vector<CZVector3D<float>> &vertVector, int count)const
{
	//ÿ�δ�vertVector��ȡ��3�����㣬�����ΪiVert{+0, +1, +2}
	for (int iVert = 0; iVert < count ; iVert+=3)
	{
		//һ������3�����㣺v1��v2��v3
		const CZVector3D<float>& v1 = vertVector[iVert];
		const CZVector3D<float>& v2 = vertVector[iVert + 1];
		const CZVector3D<float>& v3 = vertVector[iVert + 2];

		//��������2������������v=v1->v2������w=v1->v3
		CZVector3D<float> v(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
		CZVector3D<float> w(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);

		//����v �� ����w ������������������vn
		CZVector3D<float> vn(v.y*w.z - v.z*w.y, v.z*w.x - v.x*w.z, v.x*w.y - v.y*w.x);

		//�Է�����vn��λ��
		float length = sqrtf(vn.x*vn.x + vn.y*vn.y + vn.z*vn.z);
		if (0 < length) {
			const float a = 1 / length;
			vn.x *= a;
			vn.y *= a;
			vn.z *= a;
		}

		pNormVector->push_back(vn);
	}
}