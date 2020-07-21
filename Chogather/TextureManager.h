#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <glad/glad.h>
class Texture2D
{
public:
	unsigned int ID;
	unsigned int Width, Height;
	unsigned int Internal_Format; 
	unsigned int Image_Format;

	unsigned int Wrap_S; 
	unsigned int Wrap_T;
	unsigned int Filter_Min; 
	unsigned int Filter_Max;
	Texture2D();
	Texture2D(unsigned int internal_format, unsigned int image_format, unsigned int wrap_s, unsigned int wrap_t, unsigned int filter_min, unsigned int filter_max);
	void Generate(const char* path);
	void Bind() const;
};

#endif