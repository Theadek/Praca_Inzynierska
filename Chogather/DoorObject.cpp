#include "DoorObject.h"

Model* DoorObject::doorModel = NULL;

DoorObject::DoorObject(glm::vec2 position, glm::vec3 scale, GLfloat rotate) {
    if (!doorModel) {
        doorModel = new Model("Models/Door/door.obj");
    }
    GraphicsObject* graphicsObject = new GraphicsObject(glm::vec3(position, positionOnZ), scale, rotate, doorModel);
    PhysicsObject* physicsObject = new PhysicsObject(graphicsObject, 1.0f);
    this->object = new Object(graphicsObject, physicsObject, DOOR);
    this->object->physicsObject->pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
    this->object->physicsObject->pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
    this->object->physicsObject->pRigidBody->setFriction(1.0f);
    this->object->physicsObject->pRigidBody->setRestitution(0.0f);
    this->isOpen = false;
    this->isTriggered = false;
    this->controllerID = -1;
    this->anim = framesForAnimation;
}

void DoorObject::update() {
    if (isOpen)
        object->physicsObject->pRigidBody->setCollisionFlags(object->physicsObject->pRigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    else
        object->physicsObject->pRigidBody->setCollisionFlags(object->physicsObject->pRigidBody->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);

    if (isTriggered) {
        if (anim >= 0 && !isOpen) {
            object->graphicsObject->position.z -= object->graphicsObject->model->size.z / (framesForAnimation/2);
            anim--;
        }
        else if (isOpen && anim <= framesForAnimation) {
            object->graphicsObject->position.z+= object->graphicsObject->model->size.z / (framesForAnimation/2);
            anim++;
        }
        if (anim == 0 || anim == framesForAnimation) {
            isTriggered = false;
            isOpen = !isOpen;
        }
    }

}
