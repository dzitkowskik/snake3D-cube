#include "TextureManager.h"
#include "Angel.h"
#include "TextureLoader.h"

GLuint TextureManager::loadCubemapTexture(const char* cube[], GLenum types[])
{
	GLuint cubemapTexture;
	glActiveTexture(GL_TEXTURE0); 
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    for (unsigned int i = 0; i < 6; i++) 
	{
		unsigned int nWidth, nHeight;
        void* image = loadBMPRaw(cube[i], nWidth, nHeight);
        glTexImage2D(
			types[i],
			0, //level
			GL_RGB8, //internal format
			nWidth, //width
			nHeight, //height
			0, //border
			GL_BGR, //format - Not BMP uses BGR not RGB
			GL_UNSIGNED_BYTE, //type
			image);

        delete [] image;
    } 

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return cubemapTexture;
}

GLuint TextureManager::initCubemapTexture() {
	
	const char* cube[] = 
	{
		"textures/posx.bmp", 
		"textures/negx.bmp", 
		"textures/posy.bmp", 
		"textures/negy.bmp", 
		"textures/posz.bmp", 
		"textures/negz.bmp" 
	};

	GLenum types[] = 
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	return loadCubemapTexture(cube, types);
}
