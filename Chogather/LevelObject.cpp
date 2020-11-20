#include "LevelObject.h"

LevelObject::LevelObject(Model* model, glm::vec2 diamondPosition, Camera* camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT) {
    GraphicsObject* levelGraphicsObject = new GraphicsObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, model);
    PhysicsObject* levelPhysicsObject = new PhysicsObject(levelGraphicsObject, 0.0f);
    Object* level = new Object(levelGraphicsObject, levelPhysicsObject, LEVEL);
    this->object = level;
    this->object->physicsObject->pRigidBody->setLinearFactor(btVector3(1.0f, 1.0f, 0.0f));
    this->object->physicsObject->pRigidBody->setFriction(1.0f);
    this->object->physicsObject->pRigidBody->setRestitution(0.0f);
    this->diamond = new DiamondObject(diamondPosition);
    this->camera = camera;
    this->SCR_WIDTH = SCR_WIDTH;
    this->SCR_HEIGHT = SCR_HEIGHT;
}

bool LevelObject::isOnTheGround(Hero* hero) {
    float positionX = hero->object->graphicsObject->position.x;
    float positionY = hero->object->graphicsObject->position.y;
    float positionZ = hero->object->graphicsObject->position.z;
    float halfOfSizeWithScaleY = hero->object->graphicsObject->model->size.y * hero->object->graphicsObject->scale.y / 2;
    float halfOfSizeWithScaleX = hero->object->graphicsObject->model->size.x * hero->object->graphicsObject->scale.x / 2;
    btCollisionWorld::ClosestRayResultCallback rayCallbackCenter(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY - halfOfSizeWithScaleY - 0.1, positionZ));
    btCollisionWorld::ClosestRayResultCallback rayCallbackLeft(btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY - 0.1, positionZ));
    btCollisionWorld::ClosestRayResultCallback rayCallbackRight(btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY - 0.1, positionZ));

    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY - halfOfSizeWithScaleY - 0.1, positionZ), rayCallbackCenter);
    m_pWorld->rayTest(btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY - 0.1, positionZ), rayCallbackLeft);
    m_pWorld->rayTest(btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY - 0.1, positionZ), rayCallbackRight);

    return (rayCallbackCenter.hasHit() || rayCallbackLeft.hasHit() || rayCallbackRight.hasHit());
}

bool LevelObject::isBelow(Object* object) {
    float positionX = object->graphicsObject->position.x;
    float positionY = object->graphicsObject->position.y;
    float positionZ = object->graphicsObject->position.z;
    float halfOfSizeWithScaleY = object->graphicsObject->model->size.y * object->graphicsObject->scale.y / 2;
    btCollisionWorld::AllHitsRayResultCallback rayCallbackCenter(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY + halfOfSizeWithScaleY, positionZ));
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY + halfOfSizeWithScaleY, positionZ), rayCallbackCenter);
    if (rayCallbackCenter.m_collisionObjects.size() == 3) {  // 1 for floor, 1 for itself, 1 for object on the top
        return true;
    }
    else {
        return false;
    }
}

bool LevelObject::isOnTheWallLeft(Hero* hero) {
    float positionX = hero->object->graphicsObject->position.x;
    float positionY = hero->object->graphicsObject->position.y;
    float positionZ = hero->object->graphicsObject->position.z;
    float halfOfSizeWithScaleX = hero->object->graphicsObject->model->size.x * hero->object->graphicsObject->scale.x / 2;
    btCollisionWorld::ClosestRayResultCallback rayCallbackLeft(btVector3(positionX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ));
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ), rayCallbackLeft);
    if (rayCallbackLeft.hasHit()) {
        return true;
    }
    return false;

}

bool LevelObject::isOnTheWallRight(Hero* hero) {
    float positionX = hero->object->graphicsObject->position.x;
    float positionY = hero->object->graphicsObject->position.y;
    float positionZ = hero->object->graphicsObject->position.z;
    float halfOfSizeWithScaleX = hero->object->graphicsObject->model->size.x * hero->object->graphicsObject->scale.x / 2;
    btCollisionWorld::ClosestRayResultCallback rayCallbackRight(btVector3(positionX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ));
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ), rayCallbackRight);
    if (rayCallbackRight.hasHit()) {
        return true;
    }
    return false;
}

bool LevelObject::isInTheDepth(Hero* hero) {
    float positionX = hero->object->graphicsObject->position.x;
    float positionY = hero->object->graphicsObject->position.y;
    float positionZ = hero->object->graphicsObject->position.z;
    btCollisionWorld::ClosestRayResultCallback rayCallbackCenter(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY, DEPTH_TEST_RANGE));
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY, DEPTH_TEST_RANGE), rayCallbackCenter);
    if (rayCallbackCenter.hasHit()) {
        //We have to store object position bcs we dont have our rayCast system, later we will check  if it is lever
        btTransform trans = rayCallbackCenter.m_collisionObject->getWorldTransform();
        hero->leverPos = glm::vec2(trans.getOrigin().getX(), trans.getOrigin().getY());
        //cout << "Smth is in the depth" << endl;
        return true;
    }
    else {
        //cout << "Nothing is there!" << endl;
        return false;
    }
}

bool LevelObject::isInTheEnd(Hero* hero) {
    float positionX = hero->object->graphicsObject->position.x;
    float positionY = hero->object->graphicsObject->position.y;
    float positionZ = hero->object->graphicsObject->position.z;
    float halfOfSizeWithScaleX = hero->object->graphicsObject->model->size.x * hero->object->graphicsObject->scale.x / 2;
    btCollisionWorld::AllHitsRayResultCallback rayCallbackLeft(btVector3(positionX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ));
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ), rayCallbackLeft);
    btCollisionWorld::AllHitsRayResultCallback rayCallbackRight(btVector3(positionX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ));
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ), rayCallbackRight);
    if (rayCallbackLeft.hasHit()) {
        btTransform trans = rayCallbackLeft.m_collisionObject->getWorldTransform();
        hero->diamondPos = glm::vec2(trans.getOrigin().getX(), trans.getOrigin().getY());
        return true;
    }
    else if (rayCallbackRight.hasHit()) {
        btTransform trans = rayCallbackRight.m_collisionObject->getWorldTransform();
        hero->diamondPos = glm::vec2(trans.getOrigin().getX(), trans.getOrigin().getY());
        return true;
    }
    else {
        return false;
    }
}