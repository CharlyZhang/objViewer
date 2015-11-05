#include "CZRenderer.h"
#include "CZDefine.h"
#include "CZLog.h"

using namespace std;

CZRenderer::~CZRenderer()
{
	for (auto iterPShader = m_shaders.begin(); iterPShader != m_shaders.end(); iterPShader++)
		delete iterPShader->second;
}

void CZRenderer::addShader(int shadingOption, CZShader* pShader)
{
	if (m_shaders.find(shadingOption) != m_shaders.end())
		LOG_ERROR("�ظ�������shader��shadingOption��%d", shadingOption);
	else
		m_shaders.insert(pair<int, CZShader*>(shadingOption, pShader));
}

void CZRenderer::loadShader(int shadingOption, const char* vertFileName, const char* fragFileName)
{
	CZShader *pShader = getShader(shadingOption);
	if (pShader == nullptr)
		LOG_ERROR("û�д���shader��shadingOption\"%d\"", shadingOption);
	else
		pShader->load(vertFileName, fragFileName);
}

void CZRenderer::prepare(CZObjModel &model)
{
	
}


CZShader* CZRenderer::getShader(int shadingOption) const
{
	
}