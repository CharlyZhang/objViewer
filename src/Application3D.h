
#include <string>

class Application3D 
{
public:
	Application3D(){};
	~Application3D(){};

	bool init(int width, int height);
	bool loadScene((const std::string &filename);
	void frame();
	void reset();

	//
	bool loadObjModel(const std::string &filename);
	void setBackgroundColor(float r, float g, float b, float a);
	void setModelColor(float r, float g, float b, float a);
	// light
	void setLightPosition(float x, float y, float z);
	void setLightDiffuse(float r, float g, float b, float a);

	// texture
	bool enableTexture(bool flag);
};