#include <iostream>
#include "TextureManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>

Texture2D::Texture2D() {
    Width = 0;
    Height = 0;
    Internal_Format = GL_RGB;
    Image_Format = GL_RGB;
    Wrap_S = GL_REPEAT;
    Wrap_T = GL_REPEAT;
    Filter_Min = GL_LINEAR;
    Filter_Max = GL_LINEAR;
	glGenTextures(1, &ID);
	glGenerateMipmap(GL_TEXTURE_2D);
}
Texture2D::Texture2D(unsigned int internal_format = GL_RGB, unsigned int image_format = GL_RGB,
	unsigned int wrap_s = GL_REPEAT, unsigned int wrap_t = GL_REPEAT, unsigned int filter_min = GL_LINEAR, unsigned int filter_max = GL_LINEAR)
{
    Width = 0;
    Height = 0;
    Internal_Format = internal_format;
    Image_Format = image_format;
    Wrap_S = wrap_s;
    Wrap_T = wrap_t;
    Filter_Min = filter_min;
    Filter_Max = filter_max;
	glGenTextures(1, &ID);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::Generate(const char* path)
{
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	Width = width;
	Height = height;
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, Internal_Format, width, height, 0, Image_Format, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture2D::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}