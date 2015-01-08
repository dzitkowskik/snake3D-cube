#include "LightManager.h"
#include "Uniform.h"
#include <iostream>
#include <sstream>

using namespace std;

template<typename T>
void LightManager::setLightUniform(const char* uniformName, size_t index, const T& value)
{
	std::ostringstream ss;
	ss << "Lights[" << index << "]." << uniformName;
	std::string name = ss.str();
	_Uniform<T> uniformSetter;

	uniformSetter.Register(this->shader.shaderProgram, name.c_str());
	uniformSetter.SetData(value);
}

void LightManager::loadLights()
{
	this->lights.clear();

	Light first;
	first.position = -lightPos;
	first.color = vec3(.75, .75, .75);
	first.ambientCoefficient = 0.08f;
	first.attenuation = 0.001f;
	first.lightType = 1.0;
	this->lights.push_back(first);

	Light second;
	second.position = lightPos;
	second.color = vec3(.75, .75, .75);
	second.ambientCoefficient = 0.08f;
	second.attenuation = 0.001f;
	second.lightType = 1.0;
	lights.push_back(second);


	for(size_t i = 0; i < lights.size(); ++i)
	{
		setLightUniform("position",i,lights[i].position);
		setLightUniform("color",i,lights[i].color);
		setLightUniform("lightType",i,lights[i].lightType);
		setLightUniform("attenuation",i,lights[i].attenuation);
		setLightUniform("ambientCoefficient",i,lights[i].ambientCoefficient);
	}

	glUniform1i( glGetUniformLocation(this->shader.shaderProgram, "NumLights"), lights.size() );
}