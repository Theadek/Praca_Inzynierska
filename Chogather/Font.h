#pragma once
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include "Shader.h"
#include <string>


struct Character {
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

class Font
{
public:
    Font(const char* font_path);
    void RenderText(Shader* shader, std::string text, float x, float y, float scale, glm::vec3 color);


private:
    GLuint VAO, VBO;
    std::map<char, Character> Characters;
};

