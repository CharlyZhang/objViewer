#include "ObjFileLoader.h"
#include "CZLog.h"

#include <fstream>
#include <string>

using namespace std;

void CObjFileParser::load(const string& path)
{
	//���ļ�β�����ļ�������ͳ���ļ���С��֮��ص��ļ�ͷ��
	ifstream ifs(path.c_str(), ios::in | ios::ate);
	if (!ifs)
    {
		LOG_WARN("�ļ�������");
        return;
    }

	m_dir = path.substr(0, path.find_last_of('/'));

	const int fileSize = ifs.tellg();
	ifs.seekg(0, ios::beg);

	if (0 == fileSize)
		LOG_WARN("�ļ���û������");

	//	explain what's going on
	LOG_INFO("Parsing file %s  (size = %d bytes)...\n", path.c_str(), fileSize);

	// and go.
	static int lastPercent = 0;
	int percent = 10;	//	progress indicator
	while (skipCommentLine(ifs))
	{
		// show progress for files larger than one Mo
		if ((fileSize > 1024 * 1024) && (100 * ifs.tellg() / fileSize >= percent)) {
			percent = 100 * ifs.tellg() / fileSize;
			percent = (percent / 10) * 10;

			if (lastPercent != percent)
			{
				LOG_INFO("���ȣ��ٷ�֮%d\n", percent);
				lastPercent = percent;
			}
			
		}

		string ele_id;
		if (!(ifs >> ele_id))
			break;
		else
			parseLine(ifs, ele_id);
	}
}

void CObjFileParser::skipLine(ifstream& is)
{
	char next;
	is >> std::noskipws;
	while ((is >> next) && ('\n' != next));
}

bool CObjFileParser::skipCommentLine(ifstream& is)
{
	char next;
	while (is >> std::skipws >> next)
	{
		is.putback(next);
		if ('#' == next)
			skipLine(is);
		else
			return true;
	}
	return false;
}

/*�÷���ifstream::operator >> (int&)
*�Ⱥ������ɿհ׷���Ȼ�������¼���������в�ͬ�����
*								|			  ��ȡ��			|		   clear()��	  
*�����������ʽ��	����	|��ȡ��	good()?	eof()?	peek()	|good()?	eof()?	peek()
*<digit>+ <������>		123a	|T		T		F		a		|
*<digit>0 <������>		a		|F		F		F		EOF		|T			F		a
*<digit>+ <�ļ�β>		123β	|T		F		T		EOF		|T			F		EOF
*<digit>0 <�ļ�β>		β		|F		F		T		EOF		|T			F		EOF 
*
*ʵ��parseNumberElement()ʱ����Ҫ���������������������δ�������*/
int CObjFileParser::parseNumberElement(ifstream &ifs, int *pData, char sep, int defaultValue, int maxCount)
{
	int count = 0;
	int data;
	char c;//���������ַ�

	/*ÿ��ѭ�������������ݣ�
	*1.count�Ƿ�+1���Ƿ�Ҫ��Ĭ��ֵ���뵱ǰ���ݴ�
	*2.�Ƿ��������һ�ַ�Ϊsep����������������
	*3.�Ƿ���Ҫifs.clear()*/
	while (true){
		ifs >> data;
		if (ifs.good()){//���������һ��<digit>+ <������>��
			pData[count++] = data;
			if (ifs.peek() != sep)
				break;
			else
				ifs.get(c);
		}
		else{
			if (!ifs.eof()){//�������������<digit>0 <������>��
				ifs.clear();
				pData[count++] = defaultValue;
				if (ifs.peek() != sep)
					break;
				else
					ifs.get(c);
			}
			else{//�����ļ�β����������ģ��������Ƿ�������ݶ����䶪��
				ifs.clear();
				break;
			}
		}
	}

	return count;
}