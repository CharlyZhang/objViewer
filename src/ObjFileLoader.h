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
	//ע������ʾ�ٷֱȡ����ַ��̰߳�ȫ����Ϊ����static����
	virtual void load(const string& path);

protected:
	//@param is �ļ�����������ȡ��Ч���ݣ�������ǰ׺��
	//@param ele_id �����е�ǰ׺���ݴ˾�����ȡ��Щ����
	virtual void parseLine(ifstream& ifs, const string& ele_id)
	{
		/*�������������д���һ��ʵ�֣� */
		//if ("mtllib" == ele_id) {
		//	string mtlFilePath;
		//	ifs >> mtlFilePath;
		//	cout << "	mtllib " << (m_dir + "/" + mtlFilePath) << endl;
		//}
		//else if ("v" == ele_id) {	//vertex data
		//	float x, y, z;
		//	ifs >> x >> y >> z;
		//	m_pGeometry->addVertex(SVertex3f(x, y, z));
		//}
		//else
		//	skipLine(is);
	}

	void skipLine(ifstream& is);
	bool skipCommentLine(ifstream& is);

	/*��ȡһ��Ԫ�أ������������ҷǷָ����ķ���֮������ݣ������е�����
	*	һ��Ԫ�ص�������@param sepΪ�ָ����������ָ���֮����û�����ݣ�������ֵΪ@param defaultValue
	*	��ȡÿ���ָ���֮ǰ������
	*	��ȡ����ķ������ҷǷָ���ǰ�����ݣ�Ȼ��ֹͣ��ȡ
	*	����ȡ�����ļ�β������
	*@require pData���������㹻���ڴ档
	*@param maxCount ����ȡ���ٸ����ݾ�ֹͣ�����ڷ�ֹ���
	*@return ���ݸ���
	*@promise ifs������ָ��ָ������ķ������ҷǷָ�������EOF�������ַ���ͨ��ifs.peek()�鿴
	*@promise ifs.good()Ϊtrue
	*
	*��������
	* ����			data
	* 1/2/3�ո�		 1,  2, 3
	* 1//3�ո�		 1, -1, 3
	* 1/2�ո�		 1,  2
	* 1�ո�			 1
	* /β				-1
	* 1//3β		 1, -1
	* 1/2β			 1		*/
	int CObjFileParser::parseNumberElement(ifstream &ifs, int *pData, char sep = '/', int defaultValue = -1, int maxCount = 3);

	string m_dir;//���·��(�Գ����Ŀ¼Ϊ��Ŀ¼��
};