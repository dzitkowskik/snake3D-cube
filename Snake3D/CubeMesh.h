#include "Shader.h"
#include "Game.h"
#include "Camera.h"
#include <vector>

using namespace std;

#pragma once
class CubeMesh
{
public:
	Shader shader;
	GLuint vertexArrayObject;
	vector<int> indices;
	string name;

public:
	CubeMesh(void){}
	CubeMesh(Shader shader, const char* name)
		:shader(shader),name(name) 
	{
		activate();
		loadModel(); 
		deactivate();
	}
	~CubeMesh(void){}

public:
	void activate();
	void deactivate();
	void draw(Cube cube, Camera camera, float material_shininess=.5);
	void drawCubeMap(Cube cube, Camera camera, GLuint cubemapTexture);

private:
	void loadModel();
};

