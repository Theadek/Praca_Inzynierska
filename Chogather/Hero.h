#pragma once
#include "Object.h"
#define SPEED 3.0
#define JUMP_HEIGHT 10.0
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
    float speed, jump_height;
    Hero(Object* object);
    void Move(Movement playerChoice);
};

