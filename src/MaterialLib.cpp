#include "MaterialLib.h"

#include <string>
using namespace std;

void CMaterialLib::parseLine(ifstream& ifs, string ele_id)
{
	if ("newmtl" == ele_id) {
		string mtlName;
		ifs >> mtlName;
		cout << "newmtl " << mtlName << endl;

		CZMaterial *pNewMtl = new CZMaterial();
		m_materials.insert(pair<string, CZMaterial*>(mtlName, pNewMtl));
		m_pCur = pNewMtl;
	}
	else if ("Ns" == ele_id) {	//shininess
		ifs >> m_pCur->Ns;
	}
	else if ("Ka" == ele_id) {	//ambient color
		ifs >> m_pCur->Ka[0] >> m_pCur->Ka[1] >> m_pCur->Ka[2];
		ifs.clear();
	}
	else if ("Kd" == ele_id) {	//diffuse color
		ifs >> m_pCur->Kd[0] >> m_pCur->Kd[1] >> m_pCur->Kd[2];
		ifs.clear();
	}
	else if ("Ks" == ele_id) {	//specular color
		ifs >> m_pCur->Ks[0] >> m_pCur->Ks[1] >> m_pCur->Ks[2];
		ifs.clear();
	}
	else if ("map_Kd" == ele_id) {
		string texImgPath;
		ifs >> texImgPath;//����ͼ���·�����Ը�mtl�ļ�����Ŀ¼Ϊ����

		texImgPath = m_dir + "/" + texImgPath;//ת��������ڳ����Ŀ¼�����·��
		bool success = m_pCur->loadTexture(texImgPath.c_str());
		cout << success << endl;
	}
	else
		skipLine(ifs);
}

CZMaterial* CMaterialLib::get(string name)
{
	auto iterMtl = m_materials.find(name);
	return iterMtl != m_materials.end() ? iterMtl->second : nullptr;
}


CMaterialLib::~CMaterialLib()
{
	for (auto iterMtl = m_materials.begin(); iterMtl != m_materials.end(); iterMtl++)
	{
		delete iterMtl->second;
	}
}