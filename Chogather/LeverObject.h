#pragma once
#include "DoorObject.h"
#define positionOnZ -3.0
class LeverObject : protected Object
{
    static int counter;
    static Model* leverModel;
    static Model* closedLeverModel;
public:
    int ID;
    bool state;
    vector<DoorObject*> bindedObjects;
    Object* object;
    LeverObject(glm::vec2 position, glm::vec3 scale, GLfloat rotate);
    void bind(DoorObject* door);
    void pull();
};
