
///  \file CZShader.h
///  \brief This is the file declare the Class CZShader.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-17
///  \note

#ifndef __CZSHADER_H_
#define __CZSHADER_H_

#include <vector>
#include <string>
#include <map>

class CZShader
{
public:
	/// 不绑定属性和统一变量的shader
	CZShader(const char* vertFileName, const char* fragFileName);
	/// 绑定属性和统一变量的shader
	CZShader(const char* vertFileName, const char* fragFileName, \
		std::vector<std::string>& atrributes, std::vector<std::string>& uniforms);
	~CZShader();
	void begin();
	void end();
	unsigned int getAttributeLocation(const char* atrrName);
	unsigned int getUniformLocation(const std::string& str);
private:
	/// 销毁着色器
	void destroyShaders(unsigned int vertShader,unsigned int fragShader, unsigned int prog);
	/// 读取着色器程序
	bool textFileRead(const char *_fn, char *&_shader);
	/// 初始化OpenGL扩展
	///		\note 包含glew的初始化，应该在OpenGL和glut的初始化之后
	static bool initOpenGLExtensions();
	/// 是否支持GLSL
	static bool hasGLSLSupport();
	/// 编译程序
	bool compile();

	static bool extensionsInit;			///< 是否初始化GL扩展
	static bool useGLSL;				///< GLSL是否已经准备
	static bool bGeometryShader;		///< 是否支持G-Shader
	static bool bGPUShader4;			///< 是否支持Shader4

	char *m_VertexShader;
	char *m_FragmentShader;

	unsigned int m_Program;
	unsigned int m_Vert,m_Frag;

	bool isCompiled;					///< 是否编译
	std::map<std::string,unsigned int> m_uniforms;
};

#endif