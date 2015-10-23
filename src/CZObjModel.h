
#ifndef _CZOBJMODEL_H_
#define _CZOBJMODEL_H_

#include <string>
#include <list>
#include <fstream>
#include "CZVector.h"
#include "ObjFileLoader.h"
#include "Geometry.h"
#include "CZMaterial.h"
#include "MaterialLib.h"

using namespace std;

/// file type
typedef enum _ObjFileType {
	kNavteqObj,
	kNormalObj,
	kMqo
} ObjFileType;


/// CZObjModel
class CZObjModel : public CObjFileParser
{
public:
	CZObjModel();

	//@required 设置了openGL的二维纹理，如glEnable(GL_TEXTURE_2D);
	void load(const string& path) override;

	const bool hasNormals() const { return static_cast<int>(m_normRawVector.size()) > FIRST_INDEX; }
	const bool hasTextureCoords() const { return static_cast<int>(m_texRawVector.size()) > FIRST_INDEX; }

	const int getNumVertices() const { return static_cast<int>(m_vertRawVector.size()) - FIRST_INDEX; }
	const int getNumTextureCoords() const { return static_cast<int>(m_texRawVector.size()) - FIRST_INDEX; }
	const int getNumNormals() const { return static_cast<int>(m_normRawVector.size()) - FIRST_INDEX; }

	/*清除来自obj文件的原始数据，以节约内存
	*@side-effect 进入RENDER状态，不能再调用unpack()*/
	void clearRaw();

	void draw();

private:
	//来自obj文件的原始数据从哪个下标开始存储{0 or 1}
	//在文件《ObjModel.cpp》里设置
	static const int FIRST_INDEX;

	void parseLine(ifstream& ifs, const string& ele_id) override;

	void parseMaterialLib(std::ifstream &ifs);//mtllib <mtl文件路径>
	void parseUseMaterial(std::ifstream &ifs);//usemtl <材质名称>
	void parseVertex(std::ifstream &ifs);//v <x> <y> <z>
	void parseVertexNormal(std::ifstream &ifs);//vn <x> <y> <z>
	void parseVertexTexCoord(std::ifstream &ifs);//vt <u> <v>
	void parseFace(std::ifstream &ifs);//f <v/vt/vn <v/vt/vn> <v/vt/vn> //只处理三角形面；除了“v”，任一分量可以省略
	
	/*来自obj文件的原始数据*/
	vector<CZVector3D<float>> m_vertRawVector;
	vector<CZVector3D<float>> m_texRawVector;
	vector<CZVector3D<float>> m_normRawVector;

	CGeometry *m_pCurGeometry;
	//ObjModel托管这些Geometry对象占用的内存，不要在其他地方析构它们
	//因为有且仅有ObjModel需要管理Geometry对象，所以不必使用std::shared_ptr
	list<CGeometry*> m_geometries;

	CMaterialLib m_materialLib;
};
#endif