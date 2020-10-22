#pragma once
#include "Object.h"

enum Movement {
    JUMP = 0,
    CROUCH = 1,
    LEFT_MOVE = 2,
    RIGHT_MOVE = 3,
    INTERACTION = 4
};

enum State {
    WALKING,
    JUMPING,
    FALLING_DOWN,
    NOTHING
};
class Hero
{
public:
    Object* hero;
    float speed;
    glm::vec3 shiftVector;
    State currentState;
    float gravityCounter;
    //bool isJumping = false;
    //bool isFalling = false;
    //float currentJump;
    Hero();
    ~Hero();
    void Move(Movement, float);
};

