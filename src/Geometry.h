/*˵�������з��������޸�openGLȫ��״̬��ֱ���޸ģ������ָ���
*����ָ���Щ״̬��Ҫ�ɵ�����ʵ��*/

#ifndef __GEMORETRY_HPP__
#define __GEMORETRY_HPP__

#include "CZVector.h"
#include "CZMaterial.h"

#include <vector>

#include "CZDefine.h"

using namespace std;

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

	void addVertTexNorm(int vi, int ti, int ni)
	{
		v.push_back(vi);
		vt.push_back(ti);
		vn.push_back(ni);
	}

	vector<int> v;	//	vertex indices
	vector<int> vt;	//	texture indices
	vector<int> vn;	//	normal indices
};


class CTexture;

/*���ݲ���
 *@���� [����λ��-�ڴ�] ����ͨc++�������������ݴ洢���ڴ棨�����飩�С�����Ա���ñ�׼c++������Щ����
 *@���� [����λ��-�Դ�] ͨ��openGL���Ƶ��Դ�����ݡ�����Ա����openGL�ӿڲ�����Щ����
 *@���� [���ݸ�ʽ-ԭʼ(raw)]
 *	@��� λ������vVec������������vnVec��������������vtVec�����������ݵ�����
 *	obj��ʽ���ļ��У�һ���������ɶ��㣬һ��������3������{v, vn, vt}
 *		{v, vn, vt}ָ����{vVec, vnVec, vtVec}�и�һ�����ݡ�{v, vn, vt}���Ը�����ͬ
 *@���� [���ݸ�ʽ-���(unpacked)]
 *	@��� λ������vVec������������vnVec��������������vtVec
 *	��i������ֱ�ʹ��{vVec[i], vnVec[i], vtVec[i]}��Ϊ��{λ�ã�����������������}
 *@״̬ת�Ʊ�����ʹ�����̣�
 *״̬			���ݸ�ʽ	����λ��	��̬����			ת�Ʒ���
 *RAW����̬��	ԭʼ		�ڴ�		addXX; unpackTo		unpack->RAW_RENDER
 *RAW_RENDER	ԭʼ+���	�ڴ�+�Դ�	draw; unpackTo		clearRaw->RENDER
 *RENDER		���		�Դ�		draw				clearGMem->CLEARED
 *CLEARED		����Ч����	����Ч����
 */
class CGeometry {
public:
	/*�������Է���ͨ��
	 *@���� [��������] λ�á���ɫ�������������������
	 *	openGL�У�ÿ�����Ե��������һ�����飬������洢���ж���ĸ�����
	 *	openGL��ͼʱ����i��������������Ը�����������ĵ�i������
	 *	���������ͨ����Ű󶨣�openGL����ͨ��������Щ����
	 *		�̶������У�ÿ��ͨ���ı�Ź̶����磺λ��-0����ɫ-1��������-2����������-8
	 *		��ɫ���У�ͨ���������ɫ����attribute���ƾ���
	 *			�磺λ��-glGetAttribLocation(��ɫ������, "vertPos")����Ӧ����ɫ����������in vec3 vertPos;
	 *
	 *�÷���ȫ�ַ�Χ�����ã��Ӷ����Ժ��ÿ��Geometry���������ã�����ʵ����Χ�����ã����Ե���Geometry����������
	 *@�÷� ȫ������
	 *	ֱ������CGeometry::ATTRIB_<POS| NORM| TEX_COORD>_DEFAULT
	 *	@side-effect ÿ�ε��÷���unpack()�������������ֵ������
	 *@�÷� �ֲ�����
	 *	����bind(GLuint attribPos, GLuint attribNorm, GLuint attribTexCoord)����ʾ��ͨ��
	 *	@promise �ϴΰ󶨵�ͨ���������
	 *
	 *ע�����ÿ���л���ɫ��ʱ��������Ҫ���°�����Geometry�����ͨ������ʱֻ��ͨ�����÷���bind����ɸ�����*/
	static GLuint ATTRIB_POS_DEFAULT;
	static GLuint ATTRIB_NORM_DEFAULT;
	static GLuint ATTRIB_TEX_COORD_DEFAULT;

	CGeometry();

	~CGeometry();

	//@required �����˵�ǰ����
	//@required �����˷���"bind"
	void draw();

	void addFace(const CZFace& face) 
	{
		if (face.vn[0] != -1)
			m_hasNormals = true;
		if (face.vt[0] != -1)
			m_hasTexCoords = true;

		m_faceVector.push_back(face); 
	}
	const int getNumFaces() const { return m_faceVector.size(); }

	bool hasNormals() const{ return m_hasNormals; }
	bool hasTexCoords() const{ return m_hasTexCoords; }
    bool useTexture() const{ return m_hasTexCoords && pMaterial && pMaterial->texId != -1;}

	/*��ȡ���ŵ����ݡ���Щ���ݰ�������������
	 *	����obj�ļ���ԭʼ���ݣ�raw data����δ�������������������ǰ���������
	 *		����ԭʼ���ݵ������С�f 1/2/3 ...�����׸����㡰1/2/3���ֱ�������λ�÷����ĵ�1�����ݡ������������ĵ�2�����ݡ�������������ĵ�3������
	 *	����������������㣬���������������ֵӦ����ͬ����f 1/1/1����ʱ�൱�������������˶���1��
	 *@required û�е��ù�RemoveRaw()*/
	void unpackTo(vector<CZVector3D<float>> *pPosVector, vector<CZVector3D<float>> *pNormVector, vector<CZVector3D<float>> *pTexCoordVector,
		const vector<CZVector3D<float>> &posRawVector, const vector<CZVector3D<float>> &normRawVector, const vector<CZVector3D<float>> &texCoordRawVector) const;

	/*��ȡ���ŵ����ݣ���������Щ����������Ⱦ��Ϣ
	 *@promise ����RAW_RENDER״̬�����Խ����Geometry��Ⱦ����
	 *@promise ���÷���bind()�������ֱ�ΪATTRIB_{POS, NORM, TEX_COORD}_DEFAULT*/
	void unpack(const vector<CZVector3D<float>> &posRawVector, const vector<CZVector3D<float>> &normRawVector, const vector<CZVector3D<float>> &texCoordRawVector);

	/*������ɫ��������ͨ�������ԣ�
	 *	@side-effect �޸��˵�ǰvao��vbo�������Ҫ���ɵ����߻ָ��ֳ�*/
	void bind(GLuint attribPos, GLuint attribNorm, GLuint attribTexCoord);

	CZVector3D<float> m_minVert;
	CZVector3D<float> m_maxVert;

	string matrialName;
	CZMaterial *pMaterial;

private:
	vector<CZFace> m_faceVector;

	GLuint m_vao;
	long m_numVert;

	GLuint m_vboPos;
	GLuint m_vboNorm;
	GLuint m_vboTexCoord;

	/*vbo�ϴΰ󶨵�ͨ��*/
	GLuint m_lastAttribPos;
	GLuint m_lastAttribNorm;
	GLuint m_lastAttribTexCoord;

	/*ÿ�ε��÷���addFace()��һ����⵽�з����������������꣬�ͽ�����������Ϊtrue*/
	bool m_hasNormals;
	bool m_hasTexCoords;
    
    
    vector<CZVector3D<float>> posVector;
    vector<CZVector3D<float>> normVector;
    vector<CZVector3D<float>> texCoordVector;
};

#endif
