#include "Object.h"

Object::Object() {

}

Object::Object(GraphicsObject* graphicsObject, PhysicsObject* physicsObject, TAG tag) {
    this->graphicsObject = graphicsObject;
    this->physicsObject = physicsObject;
    this->tag = tag;
}