#pragma once
#include "Object.h"
#define positionOnZ 0.0f
class DiamondObject : protected Object
{
    static Model* diamondModel;
public:
    Object* object;
    DiamondObject(glm::vec2 position);
    void update(float deltaTime);
};
