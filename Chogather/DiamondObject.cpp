#include "DiamondObject.h"

Model* DiamondObject::diamondModel = NULL;

DiamondObject::DiamondObject(glm::vec2 position) {
    if (!diamondModel) {
        diamondModel = new Model("Models/Diamond/Diamond.obj");
    }
    GraphicsObject* graphicsObject = new GraphicsObject(glm::vec3(position, positionOnZ), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, diamondModel);
    PhysicsObject* physicsObject = new PhysicsObject(graphicsObject, 0.0f);
    this->object = new Object(graphicsObject, physicsObject, DIAMOND);
    this->object->physicsObject->pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
    this->object->physicsObject->pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
    this->object->physicsObject->pRigidBody->setFriction(1.0f);
    this->object->physicsObject->pRigidBody->setRestitution(0.0f);
    this->object->physicsObject->pRigidBody->setCollisionFlags(object->physicsObject->pRigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

void DiamondObject::update(float deltaTime) {
    object->graphicsObject->rotate += 100.0f * deltaTime;
    if (object->graphicsObject->rotate > 360.0f) {
        object->graphicsObject->rotate = 0.0f;
    }
}