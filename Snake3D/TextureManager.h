#include "Angel.h"

#pragma once
class TextureManager
{
	GLuint loadCubemapTexture(const char* cube[], GLenum types[]);
public:
	GLuint initCubemapTexture();
};

