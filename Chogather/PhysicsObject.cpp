#include "PhysicsObject.h"
#define M_PI 3.14159265358979323846
PhysicsObject::PhysicsObject(GraphicsObject* graphicsObject, float mass) {
    this->mass = mass;
    float euler = graphicsObject->rotate * M_PI / 180;
    btRotation.setEulerZYX(0.0f, euler, 0.0f);
    btPosition = btVector3(graphicsObject->position.x, graphicsObject->position.y, graphicsObject->position.z);
    pMotionState = new btDefaultMotionState(btTransform(btRotation, btPosition));
    btVector3 halfOfSizeWithScale;
    halfOfSizeWithScale.setX((graphicsObject->model->size.x / 2) * graphicsObject->scale.x);
    halfOfSizeWithScale.setY((graphicsObject->model->size.y / 2) * graphicsObject->scale.y);
    halfOfSizeWithScale.setZ((graphicsObject->model->size.z / 2) * graphicsObject->scale.z);
    pBoxShape = new btBoxShape(halfOfSizeWithScale);
    pBoxShape->calculateLocalInertia(mass, bodyInertia);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, pMotionState, pBoxShape, bodyInertia);
    pRigidBody = new btRigidBody(rbInfo);
}