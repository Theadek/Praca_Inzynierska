#pragma once
#include "Object.h"
#define positionOnZ 0.0f
class Diamond : protected Object
{
    static Model* diamondModel;
public:
    Object* object;
    Diamond(glm::vec2 position);
    void update();
};

