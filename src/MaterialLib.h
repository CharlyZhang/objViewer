#pragma once

#include "ObjFileLoader.h"
#include "CZMaterial.h"

#include <map>

using namespace std;

/*@用途 解析.mtl文件，得到数据{Key：材质名称；Value：材质对象}*/
class CMaterialLib : public CObjFileParser
{
public:
	CZMaterial* get(string name);

	CMaterialLib() {	m_pCur = NULL;	}
	~CMaterialLib();

private:
	//TODO 如果没有遇到"newmtl"行，m_pCur为nullptr时怎样处理
	void parseLine(ifstream& ifs, const string& ele_id) override;
	
	CZMaterial *m_pCur;

	//CMaterialLib托管这些CMaterial对象占用的内存，不要在其他地方析构它们
	//因为有且仅有MaterialLib需要管理CMaterial对象，所以不必使用std::shared_ptr
	map<string, CZMaterial*> m_materials;
};