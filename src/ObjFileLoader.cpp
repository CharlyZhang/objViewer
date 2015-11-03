#include "ObjFileLoader.h"
#include "CZLog.h"

#include <fstream>
#include <string>

using namespace std;

void CObjFileParser::load(const string& path)
{
	//从文件尾处打开文件，便于统计文件大小；之后回到文件头处
	ifstream ifs(path.c_str(), ios::in | ios::ate);
	if (!ifs)
    {
		LOG_WARN("文件不存在");
        return;
    }

	m_dir = path.substr(0, path.find_last_of('/'));

	const int fileSize = ifs.tellg();
	ifs.seekg(0, ios::beg);

	if (0 == fileSize)
		LOG_WARN("文件里没有数据");

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
				LOG_INFO("进度：百分之%d\n", percent);
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

/*用法：ifstream::operator >> (int&)
*先忽略若干空白符，然后在如下几种情况下有不同结果：
*								|			  读取后			|		   clear()后	  
*情况（正则表达式）	例子	|读取？	good()?	eof()?	peek()	|good()?	eof()?	peek()
*<digit>+ <非数字>		123a	|T		T		F		a		|
*<digit>0 <非数字>		a		|F		F		F		EOF		|T			F		a
*<digit>+ <文件尾>		123尾	|T		F		T		EOF		|T			F		EOF
*<digit>0 <文件尾>		尾		|F		F		T		EOF		|T			F		EOF 
*
*实现parseNumberElement()时，需要根据上述几种情况决定如何处理数据*/
int CObjFileParser::parseNumberElement(ifstream &ifs, int *pData, char sep, int defaultValue, int maxCount)
{
	int count = 0;
	int data;
	char c;//用于跳过字符

	/*每次循环处理下列内容：
	*1.count是否+1；是否要将默认值填入当前数据处
	*2.是否继续（下一字符为sep则继续；否则结束）
	*3.是否需要ifs.clear()*/
	while (true){
		ifs >> data;
		if (ifs.good()){//处理了情况一“<digit>+ <非数字>”
			pData[count++] = data;
			if (ifs.peek() != sep)
				break;
			else
				ifs.get(c);
		}
		else{
			if (!ifs.eof()){//处理了情况二“<digit>0 <非数字>”
				ifs.clear();
				pData[count++] = defaultValue;
				if (ifs.peek() != sep)
					break;
				else
					ifs.get(c);
			}
			else{//如遇文件尾（情况三、四），不论是否读到数据都将其丢弃
				ifs.clear();
				break;
			}
		}
	}

	return count;
}