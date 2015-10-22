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
		LOG_WARN("�ļ�������");

	m_dir = path.substr(0, path.find_last_of('/'));

	const int fileSize = ifs.tellg();
	ifs.seekg(0, ios::beg);

	if (0 == fileSize)
		LOG_WARN("�ļ���û������");

	//	explain what's going on
	LOG_INFO("Parsing file %s  (size = %d bytes)...\n", path, fileSize);

	// and go.
	int percent = 10;	//	progress indicator
	while (skipCommentLine(ifs))
	{
		// show progress for files larger than one Mo
		if ((fileSize > 1024 * 1024) && (100 * ifs.tellg() / fileSize >= percent)) {
			percent = 100 * ifs.tellg() / fileSize;
			percent = (percent / 10) * 10;
			LOG_INFO("���ȣ��ٷ�֮%d", percent);
		}

		string ele_id;
		if (!(ifs >> ele_id))
			break;
		else
			processLine(ifs, ele_id);
	}
}

void CObjFileParser::skipLine(istream& is)
{
	char next;
	is >> std::noskipws;
	while ((is >> next) && ('\n' != next));
}

bool CObjFileParser::skipCommentLine(istream& is)
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