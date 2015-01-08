#include "Angel.h"

#pragma once
class TextureManager
{
public:
	GLuint loadCubemapTexture(const char* cube[], GLenum types[]);
	GLuint initCubemapTexture();
};

