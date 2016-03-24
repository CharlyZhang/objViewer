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
	//TODO ���û������"newmtl"�У�m_pCurΪnullptrʱ��������
	void parseLine(std::ifstream& ifs, const std::string& ele_id) override;
	
	CZMaterial *m_pCur;

	//CZMaterialLib�й���ЩCMaterial����ռ�õ��ڴ棬��Ҫ�������ط���������
	//��Ϊ���ҽ���MaterialLib��Ҫ����CMaterial�������Բ���ʹ��std::shared_ptr
	CZMaterialMap m_materials;
};

#endif