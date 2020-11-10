#pragma once
#include "Object.h"
#define SPEED 3.0
#define JUMP_HEIGHT 12.0
enum Movement {
    JUMP = 0,
    CROUCH = 1,
    LEFT_MOVE = 2,
    RIGHT_MOVE = 3,
    INTERACTION = 4
};

enum State {
    STAYING,
    JUMPING,
    WALKING
};
class Hero
{
public:
    Object* hero;
    float speed, jump_height;
    State state;
    bool isFacingRight = true;
    Hero(Object* object);
    void Move(Movement playerChoice);
};

