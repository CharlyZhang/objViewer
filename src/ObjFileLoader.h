/*工具类。用于辅助下列类型文件的读取：
 *	.obj格式的3d模型文件
 *	.ojb文件附属的.mtl文件
 *文件特点：
 *	以字符形式组织，一行一行地读取
 *	每行均以一个单词为前缀，说明该行的意义；前缀之后有若干数据项
 *	注释行的前缀是字符“#”
 *	两行间可以有任意多数据行和空行*/

#pragma once

#include <iostream>
using namespace std;

class CObjFileParser
{
public:
	//注：“显示百分比”部分非线程安全，因为用了static变量
	virtual void load(const string& path);

protected:
	//@param is 文件流，用于提取有效数据（不含行前缀）
	//@param ele_id 数据行的前缀，据此决定提取哪些数据
	virtual void parseLine(ifstream& ifs, const string& ele_id)
	{
		/*在子类中像下列代码一样实现： */
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

	string m_dir;//相对路径(以程序根目录为根目录）
};