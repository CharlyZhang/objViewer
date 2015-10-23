#pragma once

#include "ObjFileLoader.h"
#include "CZMaterial.h"

#include <map>

using namespace std;

/*@��; ����.mtl�ļ����õ�����{Key���������ƣ�Value�����ʶ���}*/
class CMaterialLib : public CObjFileParser
{
public:
	CZMaterial* get(string name);

	CMaterialLib() {	m_pCur = NULL;	}
	~CMaterialLib();

private:
	//TODO ���û������"newmtl"�У�m_pCurΪnullptrʱ��������
	void parseLine(ifstream& ifs, const string& ele_id) override;
	
	CZMaterial *m_pCur;

	//CMaterialLib�й���ЩCMaterial����ռ�õ��ڴ棬��Ҫ�������ط���������
	//��Ϊ���ҽ���MaterialLib��Ҫ����CMaterial�������Բ���ʹ��std::shared_ptr
	map<string, CZMaterial*> m_materials;
};