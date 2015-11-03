/*说明：所有方法如需修改openGL全局状态，直接修改，均不恢复。
*如需恢复这些状态，要由调用者实现*/

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

/*数据部署：
 *@概念 [数据位置-内存] 像普通c++程序那样，数据存储在内存（如数组）中。程序员利用标准c++操作这些数据
 *@概念 [数据位置-显存] 通过openGL复制到显存的数据。程序员借助openGL接口操作这些数据
 *@概念 [数据格式-原始(raw)]
 *	@组成 位置数组vVec、法向量数组vnVec、纹理坐标数组vtVec、对上述数据的索引
 *	obj格式的文件中，一个面有若干顶点，一个顶点有3个索引{v, vn, vt}
 *		{v, vn, vt}指代了{vVec, vnVec, vtVec}中各一个数据。{v, vn, vt}可以各不相同
 *@概念 [数据格式-解包(unpacked)]
 *	@组成 位置数组vVec、法向量数组vnVec、纹理坐标数组vtVec
 *	第i个顶点分别使用{vVec[i], vnVec[i], vtVec[i]}作为其{位置，法向量，纹理坐标}
 *@状态转移表（正常使用流程）
 *状态			数据格式	数据位置	常态方法			转移方法
 *RAW（初态）	原始		内存		addXX; unpackTo		unpack->RAW_RENDER
 *RAW_RENDER	原始+解包	内存+显存	draw; unpackTo		clearRaw->RENDER
 *RENDER		解包		显存		draw				clearGMem->CLEARED
 *CLEARED		无有效数据	无有效数据
 */
class CGeometry {
public:
	/*顶点属性分量通道
	 *@概念 [顶点属性] 位置、颜色、法向量、纹理坐标等
	 *	openGL中，每个属性的数据组成一个数组，该数组存储所有顶点的该属性
	 *	openGL绘图时，第i个顶点的属性来自各个属性数组的第i个数据
	 *	属性数组和通道编号绑定，openGL根据通道解释这些数据
	 *		固定管线中，每个通道的编号固定，如：位置-0、颜色-1、法向量-2、纹理坐标-8
	 *		着色器中，通道编号由着色器的attribute名称决定
	 *			如：位置-glGetAttribLocation(着色器对象, "vertPos")；对应的着色器里声明了in vec3 vertPos;
	 *
	 *用法：全局范围地设置，从而对以后的每个Geometry对象都起作用；或者实例范围地设置，仅对单个Geometry对象起作用
	 *@用法 全局设置
	 *	直接设置CGeometry::ATTRIB_<POS| NORM| TEX_COORD>_DEFAULT
	 *	@side-effect 每次调用方法unpack()，都会根据上述值绑定数据
	 *@用法 局部设置
	 *	调用bind(GLuint attribPos, GLuint attribNorm, GLuint attribTexCoord)，显示绑定通道
	 *	@promise 上次绑定的通道被解除绑定
	 *
	 *注意事项：每次切换着色器时，可能需要重新绑定所有Geometry对象的通道！此时只能通过调用方法bind来完成该任务*/
	static GLuint ATTRIB_POS_DEFAULT;
	static GLuint ATTRIB_NORM_DEFAULT;
	static GLuint ATTRIB_TEX_COORD_DEFAULT;

	CGeometry();

	~CGeometry();

	//@required 设置了当前材质
	//@required 调用了方法"bind"
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

	/*提取重排的数据。这些数据按整个顶点索引
	 *	来自obj文件的原始数据（raw data）并未按整个顶点索引，而是按分量索引
	 *		例如原始数据的数据行“f 1/2/3 ...”的首个顶点“1/2/3”分别索引了位置分量的第1个数据、法向量分量的第2个数据、纹理坐标分量的第3个数据
	 *	如果是索引整个顶点，则各个分量的索引值应该相同，如f 1/1/1（此时相当于完整地索引了顶点1）
	 *@required 没有调用过RemoveRaw()*/
	void unpackTo(vector<CZVector3D<float>> *pPosVector, vector<CZVector3D<float>> *pNormVector, vector<CZVector3D<float>> *pTexCoordVector,
		const vector<CZVector3D<float>> &posRawVector, const vector<CZVector3D<float>> &normRawVector, const vector<CZVector3D<float>> &texCoordRawVector) const;

	/*提取重排的数据，并利用这些数据生成渲染信息
	 *@promise 进入RAW_RENDER状态，可以将这个Geometry渲染出来
	 *@promise 调用方法bind()，参数分别为ATTRIB_{POS, NORM, TEX_COORD}_DEFAULT*/
	void unpack(const vector<CZVector3D<float>> &posRawVector, const vector<CZVector3D<float>> &normRawVector, const vector<CZVector3D<float>> &texCoordRawVector);

	/*设置着色器的数据通道（属性）
	 *	@side-effect 修改了当前vao、vbo。如果需要，由调用者恢复现场*/
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

	/*vbo上次绑定的通道*/
	GLuint m_lastAttribPos;
	GLuint m_lastAttribNorm;
	GLuint m_lastAttribTexCoord;

	/*每次调用方法addFace()，一旦检测到有法向量或者纹理坐标，就将下列属性置为true*/
	bool m_hasNormals;
	bool m_hasTexCoords;
    
    
    vector<CZVector3D<float>> posVector;
    vector<CZVector3D<float>> normVector;
    vector<CZVector3D<float>> texCoordVector;
};

#endif
