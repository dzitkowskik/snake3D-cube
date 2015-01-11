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
	}
	~CubeMesh(void){}

public:
	void activate();
	void draw(Cube cube, Camera camera);
	void drawCubeMap(Cube cube, Camera camera, GLuint cubemapTexture);

private:
	void loadModel();
};

