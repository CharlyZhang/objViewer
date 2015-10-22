
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

	void addVertex(const CZVector3D<float>& v){ m_vertRawVector.push_back(v); };
	void addTextureCoord(const CZVector3D<float>& vt) { m_texRawVector.push_back(vt); }
	void addNormal(const CZVector3D<float>& vn) { m_normRawVector.push_back(vn); }

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

	void processLine(istream& is, string ele_id) override;

	/*����obj�ļ���ԭʼ����*/
	vector<CZVector3D<float>> m_vertRawVector;
	vector<CZVector3D<float>> m_texRawVector;
	vector<CZVector3D<float>> m_normRawVector;

	CGeometry *m_pCurGeometry = nullptr;
	//ObjModel�й���ЩGeometry����ռ�õ��ڴ棬��Ҫ�������ط���������
	//��Ϊ���ҽ���ObjModel��Ҫ����Geometry�������Բ���ʹ��std::shared_ptr
	list<CGeometry*> m_geometries;

	CMaterialLib m_materialLib;
};
#endif