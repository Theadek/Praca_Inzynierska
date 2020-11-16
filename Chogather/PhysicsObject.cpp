#include "PhysicsObject.h"
#define M_PI 3.14159265358979323846
PhysicsObject::PhysicsObject(GraphicsObject* graphicsObject, float mass) {
    this->mass = mass;
    float euler = graphicsObject->rotate * M_PI / 180;
    btRotation.setEulerZYX(0.0f, euler, 0.0f);
    btPosition = btVector3(graphicsObject->position.x, graphicsObject->position.y, graphicsObject->position.z);
    pMotionState = new btDefaultMotionState(btTransform(btRotation, btPosition));
    btRigidBody::btRigidBodyConstructionInfo* rbInfo;
    if (graphicsObject->model->meshCollision) {
        graphicsObject->model->pCollisionBox->setLocalScaling(btVector3(graphicsObject->scale.x, graphicsObject->scale.y, graphicsObject->scale.z));
        rbInfo = new btRigidBody::btRigidBodyConstructionInfo(mass, pMotionState, graphicsObject->model->pCollisionBox, bodyInertia);
    }
    else {
        btVector3 halfOfSizeWithScale;
        halfOfSizeWithScale.setX((graphicsObject->model->size.x / 2) * graphicsObject->scale.x);
        halfOfSizeWithScale.setY((graphicsObject->model->size.y / 2) * graphicsObject->scale.y);
        halfOfSizeWithScale.setZ((graphicsObject->model->size.z / 2) * graphicsObject->scale.z);
        pBoxShape = new btBoxShape(halfOfSizeWithScale);
        pBoxShape->calculateLocalInertia(mass, bodyInertia);
        rbInfo = new btRigidBody::btRigidBodyConstructionInfo(mass, pMotionState, pBoxShape, bodyInertia);
    }
    pRigidBody = new btRigidBody(*rbInfo);
}