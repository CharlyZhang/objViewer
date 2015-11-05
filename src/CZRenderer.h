#ifndef _CZRENDER_H_
#define _CZRENDER_H_

#include <map>
#include "CZObjModel.h"
#include "CZShader.h"

class CZRenderer
{
public:
	~CZRenderer();

	/*������ɫ�������ǲ�����������ı�
	 *@param pShader ָ��̬�ڴ档����ڴ潫��CZRenderer�йܣ������ֶ��ͷ�
	 *@param shadingOption ָ����(*pShader)����Ⱦ�������塣ÿ������ֻ��ָ��һ��shader*/
	void addShader(int shadingOption, CZShader* pShader);

	/*����һ���Ѿ�����������ɫ��������������ı�
	 *	����ı��������������ļ����Ӷ���̬�����������*/
	void loadShader(int shadingOption, const char* vertFileName, const char* fragFileName);

	/*Э��Model������Ⱦ���ݣ���Ҫ��vbo���󶨵�ͨ����
	 *	��Щ���ݱ����ڼ�������ɫ������֮�����ȷ��
	 *	��Щ�����ڸ�����ɫ��ʱ����Ҫ���¼���*/
	void prepare(CZObjModel &model);

	void render(const CZScene &scene,
		const CZMat4 &mvpMat, const CZMat4 &modelMat, const CZObjModel &model) const;

private:
	CZShader* getShader(int shadingOption) const;

	std::map<int, CZShader*> m_shaders;//�й��ⲿ���ڴ档��ȡ���ݺ�Ҫ����
};

#endif