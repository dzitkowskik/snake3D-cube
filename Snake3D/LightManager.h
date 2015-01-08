#include "Angel.h"
#include "Shader.h"
#include "Camera.h"
#include <vector>

#pragma once
struct Light 
{
	vec3 position;
	vec3 color;
	float lightType;
	float attenuation;
	float ambientCoefficient;
};

class LightManager
{
	Shader shader;
	std::vector<Light> lights;
	vec3 lightPos;
public:
	LightManager(){}
	LightManager(Shader shader):shader(shader)
	{ 
		lightPos = vec3(15,15,15);
		init(); 
	}
	~LightManager(){}
private:
	void init();
public:
	void loadLights();

	template<typename T>
	void setLightUniform(const char* uniformName, size_t index, const T& value);
};