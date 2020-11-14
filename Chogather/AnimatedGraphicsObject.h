#pragma once
#include "GraphicsObject.h"
class AnimatedGraphicsObject : public GraphicsObject
{
public:
    std::vector<Model*> modelsForJumpAnimation;
    std::vector<Model*> modelsForWalkAnimation;
    bool isWalking = false;
    bool isJumping = false;
    bool isFalling = false;
    float lastFrame;
    int currentAnimationModel;
    float timeBetweenAnimation;
    AnimatedGraphicsObject(glm::vec3 position, glm::vec3 scale, GLfloat rotate, std::vector<Model*> modelsForWalkAnimation, std::vector<Model*> modelsForJumpAnimation);
    void renderModel(Shader* shader);
private:
    void animateModel(Shader* shader);
};

