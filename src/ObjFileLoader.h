/*�����ࡣ���ڸ������������ļ��Ķ�ȡ��
 *	.obj��ʽ��3dģ���ļ�
 *	.ojb�ļ�������.mtl�ļ�
 *�ļ��ص㣺
 *	���ַ���ʽ��֯��һ��һ�еض�ȡ
 *	ÿ�о���һ������Ϊǰ׺��˵�����е����壻ǰ׺֮��������������
 *	ע���е�ǰ׺���ַ���#��
 *	���м����������������кͿ���*/

#pragma once

#include <iostream>
using namespace std;

class CObjFileParser
{
public:
	virtual void load(const string& path);

protected:
	//@param is �ļ�����������ȡ��Ч���ݣ�������ǰ׺��
	//@param ele_id �����е�ǰ׺���ݴ˾�����ȡ��Щ����
	virtual void processLine(istream& is, string ele_id)
	{
		/*�������������д���һ��ʵ�֣� */
		//if ("mtllib" == ele_id) {
		//	string mtlFilePath;
		//	is >> mtlFilePath;
		//	cout << "	mtllib " << (m_dir + "/" + mtlFilePath) << endl;
		//}
		//else if ("v" == ele_id) {	//vertex data
		//	float x, y, z;
		//	is >> x >> y >> z;
		//	m_pGeometry->addVertex(SVertex3f(x, y, z));
		//}
		//else
		//	skipLine(is);
	}

	void skipLine(istream& is);
	bool skipCommentLine(istream& is);

	string m_dir;//���·��(�Գ����Ŀ¼Ϊ��Ŀ¼��
};