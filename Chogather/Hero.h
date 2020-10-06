#pragma once
#include "Object.h"

enum Movement {
    JUMP = 0,
    CROUCH = 1,
    LEFT_MOVE = 2,
    RIGHT_MOVE = 3,
    INTERACTION = 4
};
class Hero
{
public:
    Object* hero;
    float speed;
    glm::vec3 shiftVector;
    bool isJumping = false;
    float currentJump = 3.0f;
    Hero();
    ~Hero();
    void Move(Movement, float);
};

