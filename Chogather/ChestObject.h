#pragma once
#include "Object.h"
#define positionOnZ 0.0f

class ChestObject : protected Object
{
    static Model* chestModel;
public:
    Object* object;
    ChestObject(glm::vec2 position, float mass = 1.0f);
    void update();
};

