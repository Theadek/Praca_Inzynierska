#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Model.h"
#include "PhysicsObject.h"
#include "GraphicsObject.h"

class Object
{
public:
    GraphicsObject* graphicsObject;
    PhysicsObject* physicsObject;
    TAG tag;
    Object();
    Object(GraphicsObject* graphicsObject, PhysicsObject* physicsObject, TAG tag);
    void move(int direction);
};

