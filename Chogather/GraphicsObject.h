#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Model.h"
#define WALK_ANIMATION_TIME_IN_S 1.7
#define JUMP_ANIMATION_TIME_IN_S 0.4

enum TAG {
    HERO,
    PLATFORM,
    LIGHT,
    DOOR,
    LEVER,
    PRESSURE_PLATE,
    CHEST
};
class GraphicsObject
{
public:
    glm::vec3 position;
    glm::vec3 scale;
    GLfloat rotate;
    Model* model;
    GraphicsObject(glm::vec3 position, glm::vec3 scale, GLfloat rotate, Model* model);
    virtual void renderModel(Shader* shader);
};

