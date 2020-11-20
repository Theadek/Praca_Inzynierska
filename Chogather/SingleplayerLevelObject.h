#pragma once
#include "debugDrawer.h"
#include "LevelObject.h"
#define GRAVITY -20.0

class SingleplayerLevelObject : public LevelObject
{
public:
    Hero* hero;
    SingleplayerLevelObject(Model* model, glm::vec2 diamondPosition, Camera* camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);
    void initPhysics();
    void loadLevel();
    void draw(Shader* shader);
    void update(float deltaTime);
};

