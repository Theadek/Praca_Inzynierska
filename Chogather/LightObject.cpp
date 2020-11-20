#include "LightObject.h"

Model* LightObject::lightModel = NULL;

LightObject::LightObject(glm::vec3 position) {
    if (!lightModel) {
        lightModel = new Model("Models/cube/cube.obj");
    }
    GraphicsObject* graphicsObject = new GraphicsObject(position, glm::vec3(1.5f, 1.5f, 1.5f), 0.0f, lightModel);
    PhysicsObject* physicsObject = new PhysicsObject(graphicsObject, 0.0f);
    this->object = new Object(graphicsObject, physicsObject, LIGHT);
    this->object->physicsObject->pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
    this->object->physicsObject->pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
    this->object->physicsObject->pRigidBody->setFriction(1.0f);
    this->object->physicsObject->pRigidBody->setRestitution(0.0f);
}