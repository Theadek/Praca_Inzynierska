#include "Hero.h"

Hero::Hero() {
    hero = new Object();
    hero->position.x = -5.0f;
    hero->position.y = 5.0f;
    hero->position.z = 0.0f;
    hero->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    hero->rotate = 0.0f;
    this->speed = 5.0f;
    this->shiftVector = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Hero::Move(Movement playerChoice, float deltaTime) {

    switch (playerChoice) {
    case LEFT_MOVE:
        shiftVector.x -= deltaTime * speed;
        break;
    case RIGHT_MOVE:
        shiftVector.x += deltaTime * speed;
        break;
    case JUMP:
        /*if (isJumping || isFalling) {
            break;
        }
        else {
            isJumping = true;
            isFalling = false;
        }*/
        if (currentState == WALKING) {
            currentState = JUMPING;
            gravityCounter = 3.0f;
        }
        break;
    case CROUCH:
        shiftVector.y -= deltaTime * speed;
        break;
    case INTERACTION:
        break;
    }
    //std::cout << "x:" << hero->position.x << " y: " << hero->position.y << std::endl;
}
