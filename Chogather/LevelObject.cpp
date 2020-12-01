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
    newObjects[0] = nullptr;
    newObjects[1] = nullptr;
}

bool LevelObject::isOnTheGround(Hero* hero) {
    float positionX = hero->object->graphicsObject->position.x;
    float positionY = hero->object->graphicsObject->position.y;
    float positionZ = hero->object->graphicsObject->position.z;
    float halfOfSizeWithScaleY = hero->object->graphicsObject->model->size.y * hero->object->graphicsObject->scale.y / 2;
    float halfOfSizeWithScaleX = hero->object->graphicsObject->model->size.x * hero->object->graphicsObject->scale.x / 2;
    btCollisionWorld::ClosestRayResultCallback rayCallbackCenter(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY - halfOfSizeWithScaleY, positionZ));
    btCollisionWorld::ClosestRayResultCallback rayCallbackLeft(btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY, positionZ));
    btCollisionWorld::ClosestRayResultCallback rayCallbackRight(btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY, positionZ));

    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY - halfOfSizeWithScaleY, positionZ), rayCallbackCenter);
    m_pWorld->rayTest(btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY, positionZ), rayCallbackLeft);
    m_pWorld->rayTest(btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY, positionZ), rayCallbackRight);

    return (rayCallbackCenter.hasHit() || rayCallbackLeft.hasHit() || rayCallbackRight.hasHit());
}

bool LevelObject::isBelow(Object* object) {
    float positionX = object->graphicsObject->position.x;
    float positionY = object->graphicsObject->position.y;
    float positionZ = object->graphicsObject->position.z;
    float halfOfSizeWithScaleY = object->graphicsObject->model->size.y * object->graphicsObject->scale.y / 2;
    btCollisionWorld::AllHitsRayResultCallback rayCallbackCenter(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY + halfOfSizeWithScaleY, positionZ));
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY + halfOfSizeWithScaleY, positionZ), rayCallbackCenter);
    //cout << rayCallbackCenter.m_collisionObjects.size() << endl;
    if (rayCallbackCenter.m_collisionObjects.size() == 1) {  // 1 for floor, 1 for itself, 1 for object on the top
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
    float halfOfSizeWithScaleZ = hero->object->graphicsObject->model->size.z * hero->object->graphicsObject->scale.z / 2;
    btCollisionWorld::ClosestRayResultCallback rayCallbackLeft(btVector3(positionX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ + halfOfSizeWithScaleZ));
    btCollisionWorld::ClosestRayResultCallback rayCallbackCenter(btVector3(positionX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ));
    btCollisionWorld::ClosestRayResultCallback rayCallbackRight(btVector3(positionX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ - halfOfSizeWithScaleZ));
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ + halfOfSizeWithScaleZ), rayCallbackLeft);
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ), rayCallbackCenter);
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ - halfOfSizeWithScaleZ), rayCallbackRight);
    if (rayCallbackLeft.hasHit() || rayCallbackCenter.hasHit() || rayCallbackRight.hasHit()) {
        return true;
    }
    return false;

}

bool LevelObject::isOnTheWallRight(Hero* hero) {
    float positionX = hero->object->graphicsObject->position.x;
    float positionY = hero->object->graphicsObject->position.y;
    float positionZ = hero->object->graphicsObject->position.z;
    float halfOfSizeWithScaleX = hero->object->graphicsObject->model->size.x * hero->object->graphicsObject->scale.x / 2;
    float halfOfSizeWithScaleZ = hero->object->graphicsObject->model->size.z * hero->object->graphicsObject->scale.z / 2;
    btCollisionWorld::ClosestRayResultCallback rayCallbackLeft(btVector3(positionX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ + halfOfSizeWithScaleZ));
    btCollisionWorld::ClosestRayResultCallback rayCallbackCenter(btVector3(positionX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ));
    btCollisionWorld::ClosestRayResultCallback rayCallbackRight(btVector3(positionX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ - halfOfSizeWithScaleZ));
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ + halfOfSizeWithScaleZ), rayCallbackLeft);
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ), rayCallbackCenter);
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ - halfOfSizeWithScaleZ), rayCallbackRight);
    if (rayCallbackLeft.hasHit() || rayCallbackCenter.hasHit() || rayCallbackRight.hasHit()) {
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
        cout << "Smth is in the depth: " << trans.getOrigin().getX() << " " << trans.getOrigin().getY() << endl;
        return true;
    }
    else {
        //cout << "Nothing is there!" << endl;
        return false;
    }
}

void LevelObject::resetLevel() {
    for (ChestObject* chest : chests) {
        m_pWorld->removeRigidBody(chest->object->physicsObject->pRigidBody);
        delete chest;
    }
    for (DoorObject* door : doors) {
        m_pWorld->removeRigidBody(door->object->physicsObject->pRigidBody);
        delete door;
    }
    for (LeverObject* lever : levers) {
        m_pWorld->removeRigidBody(lever->object->physicsObject->pRigidBody);
        delete lever;
    }
    for (PressurePlateObject* pressurePlate : pressurePlates) {
        m_pWorld->removeRigidBody(pressurePlate->object->physicsObject->pRigidBody);
        delete pressurePlate;
    }
    chests.clear();
    doors.clear();
    levers.clear();
    pressurePlates.clear();
    for (glm::vec2 chestPosition : initialChests) {
        ChestObject* chestObject = new ChestObject(chestPosition);
        m_pWorld->addRigidBody(chestObject->object->physicsObject->pRigidBody);
        chests.push_back(chestObject);
    }
    for (int i = 0; i < initialDoors.size(); i++) {
        DoorObject* doorObject = new DoorObject(initialDoors[i].first);
        doorObject->controllerID = initialDoors[i].second;
        m_pWorld->addRigidBody(doorObject->object->physicsObject->pRigidBody);
        doors.push_back(doorObject);
    }
    for (int i = 0; i < initialLevers.size(); i++) {
        LeverObject* leverObject = new LeverObject(initialLevers[i].first);
        leverObject->ID = initialLevers[i].second;
        for (DoorObject* door : doors) {
            if (door->controllerID == leverObject->ID) {
                leverObject->bind(door);
            }
        }
        m_pWorld->addRigidBody(leverObject->object->physicsObject->pRigidBody);
        levers.push_back(leverObject);
    }
    for (int i = 0; i < initialPressurePlates.size(); i++) {
        PressurePlateObject* pressurePlateObject = new PressurePlateObject(initialPressurePlates[i].first);
        pressurePlateObject->ID = initialPressurePlates[i].second;
        for (DoorObject* door : doors) {
            if (door->controllerID == pressurePlateObject->ID) {
                pressurePlateObject->bind(door);
            }
        }
        m_pWorld->addRigidBody(pressurePlateObject->object->physicsObject->pRigidBody);
        pressurePlates.push_back(pressurePlateObject);
    }

}

void LevelObject::addTemporaryObject(int objectType) {
    glm::vec3 lookAtVec = camera->GetViewMatrix() * glm::vec4(camera->Position, 0);
    switch (objectType) {
    case 1: {
        ChestObject* chest = new ChestObject(glm::vec2(lookAtVec.x, lookAtVec.y), 0.0f);
        chest->object->graphicsObject->debug = true;
        newObjects[0] = chest->object;
        chests.push_back(chest);
        m_pWorld->addRigidBody(chest->object->physicsObject->pRigidBody);
        break;
    }
    case 2: {
        LeverObject* lever = new LeverObject(glm::vec2(lookAtVec.x - 1.0f, lookAtVec.y));
        lever->object->graphicsObject->debug = true;
        newObjects[0] = lever->object;
        DoorObject* door = new DoorObject(glm::vec2(lookAtVec.x, lookAtVec.y));
        door->object->graphicsObject->debug = true;
        newObjects[1] = door->object;
        lever->bind(door);
        levers.push_back(lever);
        doors.push_back(door);
        m_pWorld->addRigidBody(lever->object->physicsObject->pRigidBody);
        m_pWorld->addRigidBody(door->object->physicsObject->pRigidBody);
        break;
    }
    case 3: {
        PressurePlateObject* pressurePlate = new PressurePlateObject(glm::vec2(lookAtVec.x - 1.0f, lookAtVec.y));
        pressurePlate->object->graphicsObject->debug = true;
        newObjects[0] = pressurePlate->object;
        DoorObject* door = new DoorObject(glm::vec2(lookAtVec.x, lookAtVec.y));
        door->object->graphicsObject->debug = true;
        newObjects[1] = door->object;
        pressurePlate->bind(door);
        pressurePlates.push_back(pressurePlate);
        doors.push_back(door);
        m_pWorld->addRigidBody(pressurePlate->object->physicsObject->pRigidBody);
        m_pWorld->addRigidBody(door->object->physicsObject->pRigidBody);
        break;
    }
    default:
        break;
    }
}

void LevelObject::addObject() {
    if (newObjects[0]) {
        newObjects[0]->graphicsObject->debug = false;

        if (newObjects[0]->tag == CHEST) {
            m_pWorld->removeRigidBody(newObjects[0]->physicsObject->pRigidBody);
            btVector3 inertia;
            newObjects[0]->physicsObject->pRigidBody->getCollisionShape()->calculateLocalInertia(1.0f, inertia);
            newObjects[0]->physicsObject->pRigidBody->setMassProps(1.0f, inertia);
            m_pWorld->addRigidBody(newObjects[0]->physicsObject->pRigidBody);
            initialChests.push_back(chests.back()->object->graphicsObject->position);
        }
        if (newObjects[0]->tag == LEVER) {
            initialLevers.push_back(std::make_pair(levers.back()->object->graphicsObject->position, levers.back()->ID));
        }
        if (newObjects[0]->tag == DOOR) {
            initialDoors.push_back(std::make_pair(doors.back()->object->graphicsObject->position, doors.back()->controllerID));
        }
        if (newObjects[0]->tag == PRESSURE_PLATE) {
            initialPressurePlates.push_back(std::make_pair(pressurePlates.back()->object->graphicsObject->position, pressurePlates.back()->ID));
        }
        newObjects[0] = nullptr;
        newObjects[0] = newObjects[1];
        newObjects[1] = nullptr;
    }
}