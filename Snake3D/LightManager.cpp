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

void LightManager::init()
{
	this->lights.clear();

	Light food;
	food.position = -lightPos;
	food.color = vec3(0.0, 1.0, 0.0);
	food.ambientCoefficient = 0.001f;
	food.attenuation = 0.1f;
	food.lightType = 1.0;
	this->lights.push_back(food);

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
}

void LightManager::loadLights()
{
	for(size_t i = 0; i < this->lights.size(); ++i)
	{
		setLightUniform("position",i,this->lights[i].position);
		setLightUniform("color",i,this->lights[i].color);
		setLightUniform("lightType",i,this->lights[i].lightType);
		setLightUniform("attenuation",i,this->lights[i].attenuation);
		setLightUniform("ambientCoefficient",i,this->lights[i].ambientCoefficient);
	}

	glUniform1i( glGetUniformLocation(this->shader.shaderProgram, "NumLights"), lights.size() );
}

void LightManager::updateLight(int index, vec3 position, float force)
{
	this->lights[index].position = position;
	this->lights[index].color = force * vec3(0.0, 1.0, 0.0);
}