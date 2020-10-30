#include "Object.h"

Object::Object(GraphicsObject* graphicsObject, PhysicsObject* physicsObject) {
    this->graphicsObject = graphicsObject;
    this->physicsObject = physicsObject;
}