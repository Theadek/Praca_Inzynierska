#include "PressurePlateObject.h"

int PressurePlateObject::counter = 0;
Model* PressurePlateObject::pressurePlateModel = NULL;

PressurePlateObject::PressurePlateObject() {

}

PressurePlateObject::PressurePlateObject(glm::vec2 position) {
    if (!pressurePlateModel) {
        pressurePlateModel = new Model("Models/PressurePlate/PressurePlate.obj");
    }
    GraphicsObject* graphicsObject = new GraphicsObject(glm::vec3(position, positionOnZ), glm::vec3(0.3f, 0.1f, 0.3f), 0.0f, pressurePlateModel);
    PhysicsObject* physicsObject = new PhysicsObject(graphicsObject, 1.0f);
    this->object = new Object(graphicsObject, physicsObject, PRESSURE_PLATE);
    this->object->physicsObject->pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
    this->object->physicsObject->pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
    this->object->physicsObject->pRigidBody->setFriction(1.0f);
    this->object->physicsObject->pRigidBody->setRestitution(0.0f);
    this->object->physicsObject->pRigidBody->setCollisionFlags(object->physicsObject->pRigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    this->isPressed = false;
    this->ID = counter;
    counter++;
}

void PressurePlateObject::bind(DoorObject* door) {
    bindedObjects.push_back(door);
    door->controllerID = ID;
}


void PressurePlateObject::step() {
    for (DoorObject* door : bindedObjects) {
        if(!door->isOpen)
            door->isTriggered = true;
    }

}

void PressurePlateObject::unstep() {
    for (DoorObject* door : bindedObjects) {
        if (door->isOpen)
            door->isTriggered = true;
    }

}

void PressurePlateObject::update() {
    if (isPressed) {
        step();
    }
    else {
        unstep();
    }

}