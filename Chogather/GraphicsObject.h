#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Model.h"
#define WALK_ANIMATION_TIME_IN_S 1.7
#define JUMP_ANIMATION_TIME_IN_S 0.4
class GraphicsObject
{
public:
    glm::vec3 position;
    glm::vec3 scale;
    GLfloat rotate;
    Model* model;
    std::vector<Model*> modelsForJumpAnimation;
    std::vector<Model*> modelsForWalkAnimation;
    bool isWalking = false;
    bool isJumping = false;
    bool isFalling = false;
    float lastFrame;
    int currentAnimationModel;
    float timeBetweenAnimation;
    bool sequence = true;
    GraphicsObject(glm::vec3 position, glm::vec3 scale, GLfloat rotate, Model* model);
    void renderModel(Shader* shader);
private:
    void animateModel(Shader* shader);
};

