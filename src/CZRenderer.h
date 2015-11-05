#ifndef _CZRENDER_H_
#define _CZRENDER_H_

#include <map>
#include "CZObjModel.h"
#include "CZShader.h"

class CZRenderer
{
public:
	~CZRenderer();

	/*创建着色器，但是不加载其程序文本
	 *@param pShader 指向动态内存。这块内存将由CZRenderer托管，不必手动释放
	 *@param shadingOption 指定用(*pShader)来渲染这类物体。每类物体只能指定一个shader*/
	void addShader(int shadingOption, CZShader* pShader);

	/*对于一个已经创建过的着色器，加载其程序文本
	 *	这个文本可以来自配置文件，从而动态加载所需程序*/
	void loadShader(int shadingOption, const char* vertFileName, const char* fragFileName);

	/*协助Model计算渲染数据（主要是vbo所绑定的通道）
	 *	有些数据必须在加载了着色器程序之后才能确定
	 *	有些数据在更换着色器时，需要重新计算*/
	void prepare(CZObjModel &model);

	void render(const CZScene &scene,
		const CZMat4 &mvpMat, const CZMat4 &modelMat, const CZObjModel &model) const;

private:
	CZShader* getShader(int shadingOption) const;

	std::map<int, CZShader*> m_shaders;//托管这部分内存。获取数据后不要析构
};

#endif