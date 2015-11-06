/*�����ࡣ���ڸ������������ļ��Ķ�ȡ��CZObjFileParser.h
 *	.obj��ʽ��3dģ���ļ�
 *	.ojb�ļ�������.mtl�ļ�
 *�ļ��ص㣺
 *	���ַ���ʽ��֯��һ��һ�еض�ȡ
 *	ÿ�о���һ������Ϊǰ׺��˵�����е����壻ǰ׺֮��������������
 *	ע���е�ǰ׺���ַ���#��
 *	���м����������������кͿ���*/

#ifndef _CZOBJFILEPARSER_H_
#define _CZOBJFILEPARSER_H_

#include <fstream>
#include <string>
#include "CZLog.h"

class CZObjFileParser
{
public:
	//ע������ʾ�ٷֱȡ����ַ��̰߳�ȫ����Ϊ����static����
	virtual bool load(const std::string& path);
	bool load(const char* path);

protected:
	//@param is �ļ�����������ȡ��Ч���ݣ�������ǰ׺��
	//@param ele_id �����е�ǰ׺���ݴ˾�����ȡ��Щ����
	virtual void parseLine(std::ifstream& ifs, const std::string& ele_id)
    {
        LOG_WARN("virtual function has not been impelemented!\n");
    }

	void skipLine(std::ifstream& is);
	bool skipCommentLine(std::ifstream& is);

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
	int parseNumberElement(std::ifstream &ifs, int *pData, char sep = '/', int defaultValue = -1, int maxCount = 3);

	std::string curDirPath;//���·��(�Գ����Ŀ¼Ϊ��Ŀ¼��
};

#endif
