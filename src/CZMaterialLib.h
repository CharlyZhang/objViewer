#ifndef _CZMATERIALLIB_H_
#define _CZMATERIALLIB_H_

#include "CZObjFileParser.h"
#include "CZMaterial.h"

#include <map>
typedef std::map<std::string, CZMaterial*> CZMaterialMap;

/// CZMaterial library 
class CZMaterialLib : public CZObjFileParser
{
public:
	CZMaterial* get(std::string &name);
	const CZMaterialMap& getAll();
    bool setMaterial(std::string &mtlName, CZMaterial *pMaterial);
    
	CZMaterialLib() {	m_pCur = NULL;	}
	~CZMaterialLib();

private:
	//TODO 如果没有遇到"newmtl"行，m_pCur为nullptr时怎样处理
	void parseLine(std::ifstream& ifs, const std::string& ele_id) override;
	
	CZMaterial *m_pCur;

	//CZMaterialLib托管这些CMaterial对象占用的内存，不要在其他地方析构它们
	//因为有且仅有MaterialLib需要管理CMaterial对象，所以不必使用std::shared_ptr
	CZMaterialMap m_materials;
};

#endif