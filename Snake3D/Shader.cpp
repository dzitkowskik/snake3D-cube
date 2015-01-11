#include "Shader.h"

Shader::Shader(const char* vertShader, const char* fragShader)
{
	shaderProgram = InitShader(vertShader, fragShader, "fragColor");
	glUseProgram(shaderProgram);
	// get uniform locations
	projectionUniform = glGetUniformLocation(shaderProgram, "Projection");
	modelViewUniform = glGetUniformLocation(shaderProgram, "ModelView");
	cubemapUniform = glGetUniformLocation(shaderProgram, "cubemap");
	
	// get attribute locations
	positionAttribute = glGetAttribLocation(shaderProgram, "position");
	glUseProgram(0);
}