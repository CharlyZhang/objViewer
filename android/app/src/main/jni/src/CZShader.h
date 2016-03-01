
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
    static std::string glslDirectory;
    
	/// �������Ժ�ͳһ������shader
	CZShader(const char* vertFileName, const char* fragFileName);
	/// �����Ժ�ͳһ������shader
	CZShader(const char* vertFileName, const char* fragFileName, \
		std::vector<std::string>& atrributes, std::vector<std::string>& uniforms, bool contentDirectly = false);

	~CZShader();
	void begin();
	void end();
	unsigned int getAttributeLocation(const char* atrrName);
	unsigned int getUniformLocation(const std::string& str);
	bool isReady(){ return m_ready;}

private:
	/// ������ɫ��
	void destroyShaders(unsigned int vertShader,unsigned int fragShader, unsigned int prog);
	/// ��ȡ��ɫ������
	bool textFileRead(const char *_fn, char *&_shader);
	/// ��ʼ��OpenGL��չ
	///		\note ����glew�ĳ�ʼ����Ӧ����OpenGL��glut�ĳ�ʼ��֮��
	static bool initOpenGLExtensions();
	/// �Ƿ�֧��GLSL
	static bool hasGLSLSupport();
	/// �������
	bool compile();

	static bool extensionsInit;			///< �Ƿ��ʼ��GL��չ
	static bool useGLSL;				///< GLSL�Ƿ��Ѿ�׼��
	static bool bGeometryShader;		///< �Ƿ�֧��G-Shader
	static bool bGPUShader4;			///< �Ƿ�֧��Shader4

	char *m_VertexShader;
	char *m_FragmentShader;

	unsigned int m_Program;
	unsigned int m_Vert,m_Frag;

	bool isCompiled;					///< �Ƿ����
	bool m_ready;
	std::map<std::string,unsigned int> m_uniforms;
};

#endif