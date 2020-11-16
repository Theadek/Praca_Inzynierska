#include "LeverObject.h"

int LeverObject::counter = 0;
Model* LeverObject::leverModel = NULL;
Model* LeverObject::closedLeverModel = NULL;

LeverObject::LeverObject(glm::vec2 position, glm::vec3 scale, GLfloat rotate) {
    if (!leverModel || !closedLeverModel) {
        leverModel = new Model("Models/Lever/lever.obj");
        closedLeverModel = new Model("Models/Lever/closedLever.obj");
    }
    GraphicsObject* graphicsObject = new GraphicsObject(glm::vec3(position, positionOnZ), scale, rotate, leverModel);
    PhysicsObject* physicsObject = new PhysicsObject(graphicsObject, 1.0f);
    this->object = new Object(graphicsObject, physicsObject, LEVER);
    this->object->physicsObject->pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
    this->object->physicsObject->pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
    this->object->physicsObject->pRigidBody->setFriction(1.0f);
    this->object->physicsObject->pRigidBody->setRestitution(0.0f);
    this->state = false;
    this->ID = ++counter;
}

void LeverObject::bind(DoorObject* door) {
    bindedObjects.push_back(door);
}

void LeverObject::pull() {
    bool isDoorsAvailable = false;
    for (DoorObject* door : bindedObjects) {
        if (!door->isTriggered) {
            door->isTriggered = true;
            isDoorsAvailable = true;
        }
    }
    if (isDoorsAvailable) {
        state = !state;
        object->graphicsObject->model = state ? closedLeverModel : leverModel;
    }
}