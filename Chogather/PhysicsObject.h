#pragma once
#include <glm/glm.hpp>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <btBulletCollisionCommon.h>
#include "GraphicsObject.h"

class PhysicsObject
{
public:
    btRigidBody* pRigidBody;
    btQuaternion btRotation;
    btVector3 btPosition;
    btDefaultMotionState* pMotionState;
    btCollisionShape* pBoxShape;
    btVector3 bodyInertia;
    float mass;
    PhysicsObject(GraphicsObject* graphicsObject, float mass);
};

