
#ifndef _CZOBJMODEL_H_
#define _CZOBJMODEL_H_

#include <string>
#include <vector>
#include "CZVector.h"
#include "CZObjFileParser.h"
#include "CZGeometry.h"
#include "CZMaterialLib.h"
#include "CZShader.h"
#include "CZMat4.h"

/// file type
typedef enum _ObjFileType {
	kNavteqObj,
	kNormalObj,
	kMqo
} ObjFileType;


/// CZObjModel
class CZObjModel : public CZObjFileParser
{
public:
	CZObjModel();
	~CZObjModel();

	bool load(const std::string& path) override;
    bool loadBinary(const std::string& path, const char *originalPath = NULL);
    bool saveAsBinary(const std::string& path);

	/*清除来自obj文件的原始数据，以节约内存
	*@side-effect 进入RENDER状态，不能再调用unpack()*/
	void clearRaw();

	void draw(CZShader *pShader);

private:
	void parseLine(std::ifstream& ifs, const std::string& ele_id) override;
	void parseMaterialLib(std::ifstream &ifs);		//mtllib <material lib name>
	void parseUseMaterial(std::ifstream &ifs);		//usemtl <material name>
	void parseVertex(std::ifstream &ifs);			//v <x> <y> <z>
	void parseVertexNormal(std::ifstream &ifs);		//vn <x> <y> <z>
	void parseVertexTexCoord(std::ifstream &ifs);	//vt <u> <v>
	void parseFace(std::ifstream &ifs);				//f <v/vt/vn <v/vt/vn> <v/vt/vn> 
	
	void clearRawData();
    void unpackRawData();

	// raw data
	std::vector<CZVector3D<float>> m_vertRawVector;
	std::vector<CZVector3D<float>> m_normRawVector;
    std::vector<CZVector2D<float>> m_texRawVector;
    
    std::vector<CZVector3D<float> > positions;
    std::vector<CZVector3D<float> > normals;
    std::vector<CZVector2D<float> > texcoords;

	CZGeometry *pCurGeometry;
	std::vector<CZGeometry*> geometries;
	CZMaterialLib materialLib;
	std::string mtlLibName;							///< material lib name
	
    GLuint m_vao;
    GLuint m_vboPos;
    GLuint m_vboNorm;
    GLuint m_vboTexCoord;
};
#endif