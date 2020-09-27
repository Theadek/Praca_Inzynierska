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
    Hero();
    ~Hero();
    void Move(Movement, float);
};

