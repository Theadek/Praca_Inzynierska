#pragma once
#include "Object.h"
class LightObject : protected Object
{
    static Model* lightModel;
public:
    Object* object;
    LightObject(glm::vec3 position, glm::vec3 scale, GLfloat rotate);
};

