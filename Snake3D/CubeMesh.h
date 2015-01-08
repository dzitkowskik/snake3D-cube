#include "Shader.h"
#include "Game.h"
#include "Camera.h"
#include <vector>
#include "LightManager.h"

using namespace std;

#pragma once
class CubeMesh
{
public:
	Shader shader;
	GLuint vertexArrayObject;
	vector<int> indices;
	string name;
	LightManager lights;

public:
	CubeMesh(void){}
	CubeMesh(Shader shader, const char* name)
		:shader(shader),name(name) 
	{
		glUseProgram(this->shader.shaderProgram);
		lights = LightManager(shader);
		loadModel(); 
	}
	~CubeMesh(void){}

public:
	void activate();
	void draw(Cube cube, Camera camera);

private:
	void loadModel();
};

