#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "TextureManager.h"
#include "Shader.h"
class Background
{
public:
    GLfloat vertices[20] = {
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f,    //right top
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f,   // right bottom
        -1.0f, -1.0f,-1.0f,  0.0f, 0.0f,  //left bottom
        -1.0f, 1.0f,-1.0f,  0.0f, 1.0f    //left top
    };
    GLuint indices[6] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
    };
    GLuint VBO, VAO, EBO;
    Shader* backgroundShader;
    Texture2D* backgroundTexture;
    Background();
    void draw();
};

