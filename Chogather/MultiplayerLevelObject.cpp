#include "MultiplayerLevelObject.h"

MultiplayerLevelObject::MultiplayerLevelObject(Model* model, glm::vec2 diamondPosition, Camera* camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT) : LevelObject(model, diamondPosition, camera, SCR_WIDTH, SCR_HEIGHT) {

}


void MultiplayerLevelObject::initPhysics() {
    m_pBroadphase = new btDbvtBroadphase();
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
    m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
    m_pSolver = new btSequentialImpulseConstraintSolver;
    m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);
    m_pWorld->setGravity(btVector3(0.0, GRAVITY, 0.0));
    debugDrawerObject = new debugDrawer(camera, SCR_WIDTH, SCR_HEIGHT);
}

void MultiplayerLevelObject::loadLevel() {
    initPhysics();
    for (DoorObject* door : doors) {
        m_pWorld->addRigidBody(door->object->physicsObject->pRigidBody);
    }
    for (LeverObject* lever : levers) {
        m_pWorld->addRigidBody(lever->object->physicsObject->pRigidBody);
    }
    for (PressurePlateObject* pressurePlate : pressurePlates) {
        m_pWorld->addRigidBody(pressurePlate->object->physicsObject->pRigidBody);
    }
    for (ChestObject* chest : chests) {
        m_pWorld->addRigidBody(chest->object->physicsObject->pRigidBody);
    }
    for (LightObject* light : lights) {
        m_pWorld->addRigidBody(light->object->physicsObject->pRigidBody);
    }
    m_pWorld->addRigidBody(diamond->object->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(object->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(hero->object->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(hero2->object->physicsObject->pRigidBody);
    if (debugDrawerObject != NULL) {
        m_pWorld->setDebugDrawer(debugDrawerObject);
    }
    else {
        cout << "debugDrawerObject is not set!" << endl;
    }
}

void MultiplayerLevelObject::draw(Shader* shader) {
    diamond->object->graphicsObject->renderModel(shader);
    object->graphicsObject->renderModel(shader);
    hero->object->graphicsObject->renderModel(shader);
    hero2->object->graphicsObject->renderModel(shader);
    for (LeverObject* lever : levers) {
        lever->object->graphicsObject->renderModel(shader);
    }

    for (PressurePlateObject* pressurePlate : pressurePlates) {
        pressurePlate->object->graphicsObject->renderModel(shader);
    }

    for (DoorObject* door : doors) {
        door->object->graphicsObject->renderModel(shader);
    }

    for (ChestObject* chest : chests) {
        chest->object->graphicsObject->renderModel(shader);
    }

    for (LightObject* light : lights) {
        light->object->graphicsObject->renderModel(shader);
    }
}

void MultiplayerLevelObject::update(float deltaTime) {
    m_pWorld->stepSimulation(deltaTime);

    hero->update(isOnTheGround(hero));
    hero2->update(isOnTheGround(hero2));

    for (LeverObject* lever : levers) {
        if (hero->state == ACTION) {
            //it shoudlnt work like this, but since we dont have our rayCast system, we have to use bullet
            if (lever->object->physicsObject->btPosition.getX() == hero->leverPos.x && lever->object->physicsObject->btPosition.getY() == hero->leverPos.y) {
                lever->pull();
                hero->state = STAYING;
            }
        }
        if (hero2->state == ACTION) {
            //it shoudlnt work like this, but since we dont have our rayCast system, we have to use bullet
            if (lever->object->physicsObject->btPosition.getX() == hero2->leverPos.x && lever->object->physicsObject->btPosition.getY() == hero2->leverPos.y) {
                lever->pull();
                hero2->state = STAYING;
            }
        }
    }
    for (PressurePlateObject* pressurePlate : pressurePlates) {
        pressurePlate->isPressed = isBelow(pressurePlate->object) ? true : false;
        pressurePlate->update();
    }

    for (DoorObject* door : doors) {
        door->update();
    }

    for (ChestObject* chest : chests) {
        chest->update();
    }

    diamond->update(deltaTime);
}