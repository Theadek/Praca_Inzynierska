#pragma once
#include "DoorObject.h"
#include "LeverObject.h"
#include "PressurePlateObject.h"
#include "ChestObject.h"
#include "LightObject.h"
#include "DiamondObject.h"
#include "Hero.h"
#include "Camera.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <typeinfo>
#include <fstream>
#define GRAVITY -20.0
#define DEPTH_TEST_RANGE -5.0f
class LevelObject
{
public:
    std::vector <Object*> newObjects;
    std::vector <glm::vec2> initialChests;
    std::vector <pair<glm::vec3, int>> initialDoors, initialLevers, initialPressurePlates;
    std::vector <LeverObject*> levers;
    std::vector <DoorObject*> doors;
    std::vector <PressurePlateObject*> pressurePlates;
    std::vector <ChestObject*> chests;
    std::vector <LightObject*> lights;
    DiamondObject* diamond;
    Object* object;
    Camera* camera;
    unsigned int SCR_WIDTH, SCR_HEIGHT;
    btBroadphaseInterface* m_pBroadphase;
    btDefaultCollisionConfiguration* m_pCollisionConfiguration;
    btCollisionDispatcher* m_pDispatcher;
    btConstraintSolver* m_pSolver;
    btDynamicsWorld* m_pWorld;
    btIDebugDraw* debugDrawerObject;
    LevelObject(Model* model, glm::vec2 diamondPosition, Camera* camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);
    LevelObject(Model* model, string path, Camera* camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);
    virtual void initPhysics() = 0;
    virtual void loadLevel() = 0;
    virtual void draw(Shader* shader) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void saveLevel() = 0;
    virtual void loadLevelFromFile(string path) = 0;
    bool isOnTheGround(Hero* hero);
    bool isBelow(Object* object);
    bool isOnTheWallLeft(Hero* hero);
    bool isOnTheWallRight(Hero* hero);
    bool isInTheDepth(Hero* hero);
    void addTemporaryObject(TAG objectType);
    void addObject();
    void resetLevel();

};

