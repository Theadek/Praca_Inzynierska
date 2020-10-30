#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Model.h"
class GraphicsObject
{
public:
    glm::vec3 position;
    glm::vec3 scale;
    GLfloat rotate;
    Model* model;
    GraphicsObject(glm::vec3 position, glm::vec3 scale, GLfloat rotate, Model* model);
    void renderModel(Shader* shader);
};

