#include "Hero.h"

Hero::Hero() {
    hero = new Object();
    hero->position.x = 0.0f;
    hero->position.y = 0.0f;
    hero->position.z = 0.0f;
    hero->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    hero->rotate = 0.0f;
    this->speed = 5.0f;
}

void Hero::Move(Movement playerChoice, float deltaTime) {
    switch (playerChoice) {
    case LEFT_MOVE:
        hero->position -= glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime * speed;
        break;
    case RIGHT_MOVE:
        hero->position += glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime * speed;
        break;
    case JUMP:
        hero->position += glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime * speed;
        break;
    case CROUCH:
        hero->position -= glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime * speed;
        break;
    case INTERACTION:
        break;
    }
    std::cout << "x:" << hero->position.x << " y: " << hero->position.y << std::endl;
}