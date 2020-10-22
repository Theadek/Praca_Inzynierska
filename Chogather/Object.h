#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Model.h"

class Object
{
public:
    glm::vec3 position;
    glm::vec3 scale;
    GLfloat rotate;
    Model* model;

    void renderModel(Shader* shader, float maxX, float maxY, glm::vec3 size);
};

