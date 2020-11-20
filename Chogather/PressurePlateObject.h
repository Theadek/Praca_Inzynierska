#pragma once
#include "Object.h"
#include "DoorObject.h"
#define positionOnZ 0.0f

class PressurePlateObject : protected Object
{
    static int counter;
    static Model* pressurePlateModel;
    void step();
    void unstep();
public:
    int ID;
    bool isPressed;
    vector<DoorObject*> bindedObjects;
    Object* object;
    PressurePlateObject(glm::vec2 position);
    void bind(DoorObject* door);
    void update();

};

