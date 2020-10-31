#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Model.h"
#include "PhysicsObject.h"
#include "GraphicsObject.h"

enum TAG {
    HERO,
    PLATFORM,
    LIGHT
};
class Object
{
public:
    GraphicsObject* graphicsObject;
    PhysicsObject* physicsObject;
    TAG tag;
    Object(GraphicsObject* graphicsObject, PhysicsObject* physicsObject, TAG tag);
};

