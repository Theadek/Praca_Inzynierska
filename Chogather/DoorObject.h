#pragma once
#include "Object.h"
#define positionOnZ 0.5f
#define framesForAnimation 40
class DoorObject : protected Object
{
    static Model* doorModel;
    int anim;
public:
    int controllerID;
    bool isOpen;
    bool isTriggered;
    Object* object;
    DoorObject(glm::vec2 position);
    void update();
};

