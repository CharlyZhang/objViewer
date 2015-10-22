#include "MaterialLib.h"

#include <string>
using namespace std;

void CMaterialLib::processLine(istream& is, string ele_id)
{
	if ("newmtl" == ele_id) {
		string mtlName;
		is >> mtlName;
		cout << "newmtl " << mtlName << endl;

		CZMaterial *pNewMtl = new CZMaterial();
		m_materials.insert(pair<string, CZMaterial*>(mtlName, pNewMtl));
		m_pCur = pNewMtl;
	}
	else if ("Ns" == ele_id) {	//shininess
		is >> m_pCur->Ns;
	}
	else if ("Ka" == ele_id) {	//ambient color
		is >> m_pCur->Ka[0] >> m_pCur->Ka[1] >> m_pCur->Ka[2];
		is.clear();
	}
	else if ("Kd" == ele_id) {	//diffuse color
		is >> m_pCur->Kd[0] >> m_pCur->Kd[1] >> m_pCur->Kd[2];
		is.clear();
	}
	else if ("Ks" == ele_id) {	//specular color
		is >> m_pCur->Ks[0] >> m_pCur->Ks[1] >> m_pCur->Ks[2];
		is.clear();
	}
	else if ("map_Kd" == ele_id) {
		string texImgPath;
		is >> texImgPath;//纹理图相对路径（以该mtl文件所在目录为根）

		texImgPath = m_dir + "/" + texImgPath;//转换到相对于程序根目录的相对路径
		bool success = m_pCur->loadTexture(texImgPath.c_str());
		cout << success << endl;
	}
	else
		skipLine(is);
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