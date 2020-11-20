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
    LEFT_MOVE = 1,
    RIGHT_MOVE = 2,
    INTERACTION = 3
};

enum State {
    STAYING,
    JUMPING,
    WALKING,
    ACTION
};
class Hero
{
    static bool isLoaded;
    static std::vector<Model*> modelsForJumpAnimation;
    static std::vector<Model*> modelsForWalkAnimation;
public:
    bool isLeftWallJumpAvailable, isRightWallJumpAvailable;
    Object* object;
    float speed, jump_height;
    State state;
    glm::vec2 leverPos;
    glm::vec2 diamondPos;
    glm::vec3 lastPos;
    glm::vec2 startPosition;
    bool isFacingRight = true;
    Hero(glm::vec2 position);
    void move(Movement playerChoice);
    void update(bool isOnTheGround);
    void resetPosition();
};

