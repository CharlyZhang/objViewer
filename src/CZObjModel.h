
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

	//@required ������openGL�Ķ�ά������glEnable(GL_TEXTURE_2D);
	void load(const string& path) override;

	const bool hasNormals() const { return static_cast<int>(m_normRawVector.size()) > FIRST_INDEX; }
	const bool hasTextureCoords() const { return static_cast<int>(m_texRawVector.size()) > FIRST_INDEX; }

	const int getNumVertices() const { return static_cast<int>(m_vertRawVector.size()) - FIRST_INDEX; }
	const int getNumTextureCoords() const { return static_cast<int>(m_texRawVector.size()) - FIRST_INDEX; }
	const int getNumNormals() const { return static_cast<int>(m_normRawVector.size()) - FIRST_INDEX; }

	/*�������obj�ļ���ԭʼ���ݣ��Խ�Լ�ڴ�
	*@side-effect ����RENDER״̬�������ٵ���unpack()*/
	void clearRaw();

	void draw();

private:
	//����obj�ļ���ԭʼ���ݴ��ĸ��±꿪ʼ�洢{0 or 1}
	//���ļ���ObjModel.cpp��������
	static const int FIRST_INDEX;

	void parseLine(ifstream& ifs, const string& ele_id) override;

	void parseMaterialLib(std::ifstream &ifs);//mtllib <mtl�ļ�·��>
	void parseUseMaterial(std::ifstream &ifs);//usemtl <��������>
	void parseVertex(std::ifstream &ifs);//v <x> <y> <z>
	void parseVertexNormal(std::ifstream &ifs);//vn <x> <y> <z>
	void parseVertexTexCoord(std::ifstream &ifs);//vt <u> <v>
	void parseFace(std::ifstream &ifs);//f <v/vt/vn <v/vt/vn> <v/vt/vn> //ֻ�����������棻���ˡ�v������һ��������ʡ��
	
	/*����obj�ļ���ԭʼ����*/
	vector<CZVector3D<float>> m_vertRawVector;
	vector<CZVector3D<float>> m_texRawVector;
	vector<CZVector3D<float>> m_normRawVector;

	CGeometry *m_pCurGeometry;
	//ObjModel�й���ЩGeometry����ռ�õ��ڴ棬��Ҫ�������ط���������
	//��Ϊ���ҽ���ObjModel��Ҫ����Geometry�������Բ���ʹ��std::shared_ptr
	list<CGeometry*> m_geometries;

	CMaterialLib m_materialLib;
};
#endif