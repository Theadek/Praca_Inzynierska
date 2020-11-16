#include "ChestObject.h"

Model* ChestObject::chestModel = NULL;

ChestObject::ChestObject(glm::vec2 position, glm::vec3 scale, GLfloat rotate) {
    if (!chestModel) {
        chestModel = new Model("Models/cube/cube.obj");
    }
    GraphicsObject* graphicsObject = new GraphicsObject(glm::vec3(position, positionOnZ), scale, rotate, chestModel);
    PhysicsObject* physicsObject = new PhysicsObject(graphicsObject, 1.0f);
    this->object = new Object(graphicsObject, physicsObject, CHEST);
    this->object->physicsObject->pRigidBody->setLinearFactor(btVector3(1.0f, 1.0f, 0.0f));
    this->object->physicsObject->pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
    this->object->physicsObject->pRigidBody->setFriction(1.0f);
    this->object->physicsObject->pRigidBody->setRestitution(0.0f);
}

void ChestObject::update() {
    btTransform trans = object->physicsObject->pRigidBody->getWorldTransform();
    object->graphicsObject->position.x = trans.getOrigin().x();
    object->graphicsObject->position.y = trans.getOrigin().y();
    object->physicsObject->pRigidBody->setActivationState(ACTIVE_TAG);
}