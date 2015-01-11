#include "Angel.h"

#pragma once
class Shader
{
public:
	GLuint shaderProgram;
	GLuint projectionUniform,
		modelViewUniform,
		cubemapUniform,
		textureUniform,
		cameraPosUniform;
	GLuint positionAttribute;

	Shader(){}
	Shader(const char* vertShader, const char* fragShader);
	~Shader(){}
};

