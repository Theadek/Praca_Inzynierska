#pragma once
#include "debugDrawer.h"
#include "LevelObject.h"
class MultiplayerLevelObject : public LevelObject
{
public:
    Hero* hero;
    Hero* hero2;
    MultiplayerLevelObject(Model* model, glm::vec2 diamondPosition, Camera* camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, Shader* alternative);
    void initPhysics();
    void loadLevel();
    void draw(Shader* shader);
    void update(float deltaTime);
private:
    Shader* alternative;
};

