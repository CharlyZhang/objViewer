#include "Geometry.h"
#include "CZDefine.h"

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

//@side-effect openGL绑定了另一个VAO对象
void CGeometry::draw() const
{
    CZCheckGLError();
	pMaterial->use();

    CZCheckGLError();
	GL_BIND_VERTEXARRAY(m_vao);
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
			if (hasNormals())
				pNormVector->push_back(normRawVector[(*itFace).vn[i]]);
			else
				pNormVector->push_back(CZVector3D<float>(0, 0, 0));//FIXME 应先调用方法generateFlatNormals()
			
			if (hasTexCoords())
				pTexCoordVector->push_back(texCoordRawVector[(*itFace).vt[i]]);
			else
				pTexCoordVector->push_back(CZVector3D<float>(0, 0, 0));
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
	GL_GEN_VERTEXARRAY(1, &m_vao);
	GL_BIND_VERTEXARRAY(m_vao);
    CZCheckGLError();
    
	/*顶点位置分量对象（vbo）*/
	glGenBuffers(1, &m_vboPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboPos);
	glBufferData(GL_ARRAY_BUFFER, posVector.size() * 3 * sizeof(GLfloat), posVector.data(), GL_STATIC_DRAW);
    CZCheckGLError();
    
	//法向量分量对象（vbo）
	glGenBuffers(1, &m_vboNorm);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNorm);
	glBufferData(GL_ARRAY_BUFFER, normVector.size() * 3 * sizeof(GLfloat), normVector.data(), GL_STATIC_DRAW);
    CZCheckGLError();
    
	//纹理坐标分量对象（vbo）
	if (hasTexCoords()){
		glGenBuffers(1, &m_vboTexCoord);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboTexCoord);
		glBufferData(GL_ARRAY_BUFFER, texCoordVector.size() * 3 * sizeof(GLfloat), texCoordVector.data(), GL_STATIC_DRAW);
        CZCheckGLError();
	}

	bind(ATTRIB_POS_DEFAULT, ATTRIB_NORM_DEFAULT, ATTRIB_TEX_COORD_DEFAULT);
    CZCheckGLError();
}

void CGeometry::bind(GLuint attribPos, GLuint attribNorm, GLuint attribTexCoord)
{
	GL_BIND_VERTEXARRAY(m_vao);

	if(m_lastAttribPos != -1)    glDisableVertexAttribArray(m_lastAttribPos);
	if(m_vboNorm != -1)          glDisableVertexAttribArray(m_vboNorm);
	if(m_vboTexCoord != -1)      glDisableVertexAttribArray(m_vboTexCoord);
    CZCheckGLError();
    
	glBindBuffer(GL_ARRAY_BUFFER, m_vboPos);
	glEnableVertexAttribArray(attribPos);
	glVertexAttribPointer(attribPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	m_lastAttribPos = attribPos;
    CZCheckGLError();
    
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNorm);
	glEnableVertexAttribArray(attribNorm);
	glVertexAttribPointer(attribNorm, 3, GL_FLOAT, GL_FALSE, 0, 0);
	m_lastAttribNorm = attribNorm;
    CZCheckGLError();
    
	if (hasTexCoords()){
		glBindBuffer(GL_ARRAY_BUFFER, m_vboTexCoord);
		glEnableVertexAttribArray(attribTexCoord);
		glVertexAttribPointer(attribTexCoord, 3, GL_FLOAT, GL_FALSE, 0, 0);
		m_lastAttribTexCoord = attribTexCoord;
        CZCheckGLError();
	}
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    GL_BIND_VERTEXARRAY(0);
}