#include "GraphicsObject.h"

GraphicsObject::GraphicsObject(glm::vec3 position, glm::vec3 scale, GLfloat rotate, Model* model) {
    this->position = position;
    this->scale = scale;
    this->rotate = rotate;
    this->model = model;
    this->lastFrame = 0;
    this->currentAnimationModel = 0;
    this->timeBetweenAnimation = 0;
}

void GraphicsObject::renderModel(Shader* shader) {
    shader->use();
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
    shader->setMat4("model", modelMatrix);
    if ((modelsForWalkAnimation.size() && isWalking) || (modelsForJumpAnimation.size() && isJumping)) {
        animateModel(shader);
    }
    else {
        for (int i = 0; i < model->meshes.size(); i++) {
            model->meshes[i].draw(shader);
        }
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GraphicsObject::animateModel(Shader* shader) {
    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    timeBetweenAnimation += deltaTime;
    std::vector<Model*> modelsForAnimation;
    float ANIMATION_TIME_IN_S;
    if (isWalking) {
        modelsForAnimation = modelsForWalkAnimation;
        ANIMATION_TIME_IN_S = WALK_ANIMATION_TIME_IN_S;
    }
    else if (isJumping) {
        modelsForAnimation = modelsForJumpAnimation;
        ANIMATION_TIME_IN_S = JUMP_ANIMATION_TIME_IN_S;
    }
    if (timeBetweenAnimation >= ((float)(ANIMATION_TIME_IN_S / (modelsForAnimation.size() - 1)))) {
        timeBetweenAnimation = 0;
        currentAnimationModel++;
    }
    if (currentAnimationModel >= (modelsForAnimation.size())) {
        timeBetweenAnimation = 0;
        currentAnimationModel = 0;
    }
    for (int i = 0; i < modelsForAnimation[currentAnimationModel]->meshes.size(); i++) {
        modelsForAnimation[currentAnimationModel]->meshes[i].draw(shader);
    }
}