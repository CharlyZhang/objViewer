/*工具类。用于辅助下列类型文件的读取：CZObjFileParser.h
 *	.obj格式的3d模型文件
 *	.ojb文件附属的.mtl文件
 *文件特点：
 *	以字符形式组织，一行一行地读取
 *	每行均以一个单词为前缀，说明该行的意义；前缀之后有若干数据项
 *	注释行的前缀是字符“#”
 *	两行间可以有任意多数据行和空行*/

#ifndef _CZOBJFILEPARSER_H_
#define _CZOBJFILEPARSER_H_

#include <fstream>
#include <string>
#include "CZLog.h"

class CZObjFileParser
{
public:
	//注：“显示百分比”部分非线程安全，因为用了static变量
	virtual bool load(const std::string& path);
	bool load(const char* path);

protected:
	//@param is 文件流，用于提取有效数据（不含行前缀）
	//@param ele_id 数据行的前缀，据此决定提取哪些数据
	virtual void parseLine(std::ifstream& ifs, const std::string& ele_id)
    {
        LOG_WARN("virtual function has not been impelemented!\n");
    }

	void skipLine(std::ifstream& is);
	bool skipCommentLine(std::ifstream& is);

	/*读取一个元素（两个非数字且非分隔符的符号之间的内容）所含有的数据
	*	一个元素的数据以@param sep为分隔符。两个分隔符之间若没有数据，则视其值为@param defaultValue
	*	读取每个分隔符之前的数据
	*	读取最近的非数字且非分隔符前的数据，然后停止读取
	*	不读取紧邻文件尾的数据
	*@require pData处分配了足够的内存。
	*@param maxCount 最多读取多少个数据就停止。用于防止溢出
	*@return 数据个数
	*@promise ifs的内置指针指向最近的非数字且非分隔符（含EOF）。该字符可通过ifs.peek()查看
	*@promise ifs.good()为true
	*
	*测试用例
	* 输入			data
	* 1/2/3空格		 1,  2, 3
	* 1//3空格		 1, -1, 3
	* 1/2空格		 1,  2
	* 1空格			 1
	* /尾				-1
	* 1//3尾		 1, -1
	* 1/2尾			 1		*/
	int parseNumberElement(std::ifstream &ifs, int *pData, char sep = '/', int defaultValue = -1, int maxCount = 3);

	std::string curDirPath;//相对路径(以程序根目录为根目录）
};

#endif
