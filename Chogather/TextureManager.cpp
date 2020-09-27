#include <iostream>
#include "TextureManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>

Texture2D::Texture2D() {
	this->Width = 0;
	this->Height = 0;
	this->Internal_Format = GL_RGB;
	this->Image_Format = GL_RGB;
	this->Wrap_S = GL_REPEAT;
	this->Wrap_T = GL_REPEAT;
	this->Filter_Min = GL_LINEAR;
	this->Filter_Max = GL_LINEAR;
	glGenTextures(1, &this->ID);
	glGenerateMipmap(GL_TEXTURE_2D);
}
Texture2D::Texture2D(unsigned int internal_format = GL_RGB, unsigned int image_format = GL_RGB,
	unsigned int wrap_s = GL_REPEAT, unsigned int wrap_t = GL_REPEAT, unsigned int filter_min = GL_LINEAR, unsigned int filter_max = GL_LINEAR)
{
	this->Width = 0;
	this->Height = 0;
	this->Internal_Format = internal_format;
	this->Image_Format = image_format;
	this->Wrap_S = wrap_s;
	this->Wrap_T = wrap_t;
	this->Filter_Min = filter_min;
	this->Filter_Max = filter_max;
	glGenTextures(1, &this->ID);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::Generate(const char* path)
{
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	this->Width = width;
	this->Height = height;
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}