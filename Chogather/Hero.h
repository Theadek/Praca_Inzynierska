#pragma once
#include "Object.h"
#include "AnimatedGraphicsObject.h"
#define SPEED 3.0
#define JUMP_HEIGHT 8.0
#define positionOnZ 0.0
#define JUMP_MODELS 0
#define WALK_MODELS 20
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
    WALKING,
    CROUCHING,
    ACTION
};
class Hero
{
    static bool isLoaded;
    std::vector<Model*> modelsForJumpAnimation;
    std::vector<Model*> modelsForWalkAnimation;
public:
    bool isLeftWallJumpAvailable, isRightWallJumpAvailable;
    Object* object;
    float speed, jump_height;
    State state;
    glm::vec2 leverPos;
    glm::vec3 lastPos;
    bool isFacingRight = true;
    Hero(glm::vec2 position, glm::vec3 scale);
    void move(Movement playerChoice);
    void update(bool isOnTheGround);
};

