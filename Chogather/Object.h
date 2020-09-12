#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "ModelManager.h"

class Object
{
public:
    glm::vec3 position;
    glm::vec3 size;
    GLfloat rotate;
    Model* model;

    void renderModel(ShaderManager* shader);
};

