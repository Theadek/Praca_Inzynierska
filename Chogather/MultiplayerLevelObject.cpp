#include "MultiplayerLevelObject.h"
int MultiplayerLevelObject::counter = 1;

MultiplayerLevelObject::MultiplayerLevelObject(Model* model, glm::vec2 diamondPosition, Camera* camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, Shader* alternative) : LevelObject(model, diamondPosition, camera, SCR_WIDTH, SCR_HEIGHT) {
    this->alternative = alternative;
    this->ID = counter;
    counter++;
}

MultiplayerLevelObject::MultiplayerLevelObject(Model* model, string path, Camera* camera, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, Shader* alternative) : LevelObject(model, path, camera, SCR_WIDTH, SCR_HEIGHT) {
    this->alternative = alternative;
    this->ID = counter;
    counter++;
    loadLevelFromFile(path);
}
void MultiplayerLevelObject::initPhysics() {
    m_pBroadphase = new btDbvtBroadphase();
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
    m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
    m_pSolver = new btSequentialImpulseConstraintSolver;
    m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);
    m_pWorld->setGravity(btVector3(0.0, GRAVITY, 0.0));
    debugDrawerObject = new debugDrawer(camera, SCR_WIDTH, SCR_HEIGHT);
    m_pWorld->setDebugDrawer(debugDrawerObject);
}

void MultiplayerLevelObject::loadLevel() {
    initPhysics();
    for (DoorObject* door : doors) {
        initialDoors.push_back(std::make_pair(door->object->graphicsObject->position, door->controllerID));
    }
    for (LeverObject* lever : levers) {
        initialLevers.push_back(std::make_pair(lever->object->graphicsObject->position, lever->ID));
    }
    for (PressurePlateObject* pressurePlate : pressurePlates) {
        initialPressurePlates.push_back(std::make_pair(pressurePlate->object->graphicsObject->position, pressurePlate->ID));
    }
    for (ChestObject* chest : chests) {
        initialChests.push_back(chest->object->graphicsObject->position);
    }
    for (LightObject* light : lights) {
        m_pWorld->addRigidBody(light->object->physicsObject->pRigidBody);
    }
    m_pWorld->addRigidBody(diamond->object->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(object->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(hero->object->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(hero2->object->physicsObject->pRigidBody);
}

void MultiplayerLevelObject::draw(Shader* shader) {
    diamond->object->graphicsObject->renderModel(shader);
    object->graphicsObject->renderModel(shader);
    hero->object->graphicsObject->renderModel(shader);
    hero2->object->graphicsObject->renderModel(alternative);
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

void MultiplayerLevelObject::saveLevel() {
    std::ofstream output_file("./Levels/MultiplayerLevels/Level" + to_string(ID) + ".txt");
    output_file << "Diamond " << diamond->object->graphicsObject->position.x << " " << diamond->object->graphicsObject->position.y << " 0 0 \n";
    for (glm::vec2 chest : initialChests) {
        output_file << "Chest " << chest.x << " " << chest.y << " 0 0 \n";
    }
    for (std::pair<glm::vec3, int> door : initialDoors) {
        output_file << "Door " << door.first.x << " " << door.first.y << " " << door.first.z << " " << door.second << "\n";
    }
    for (std::pair<glm::vec3, int> lever : initialLevers) {
        output_file << "Lever " << lever.first.x << " " << lever.first.y << " " << lever.first.z << " " << lever.second << "\n";
    }
    for (std::pair<glm::vec3, int> pressurePlate : initialPressurePlates) {
        output_file << "PressurePlate " << pressurePlate.first.x << " " << pressurePlate.first.y << " " << pressurePlate.first.z << " " << pressurePlate.second << "\n";
    }
    for (LightObject* light : lights) {
        output_file << "Light " << light->object->graphicsObject->position.x << " " << light->object->graphicsObject->position.y << " " << light->object->graphicsObject->position.z << " 0\n";
    }
}

void MultiplayerLevelObject::loadLevelFromFile(string path) {
    std::ifstream input_file(path);
    std::string line;
    while (std::getline(input_file, line)) {
        std::istringstream iss(line);
        string name;
        float x, y, z, w;
        if (!(iss >> name >> x >> y >> z >> w)) {
            break;
        }
        else {
            if (name == "Diamond") {
                this->diamond = new DiamondObject(glm::vec2(x, y));
            }
            else if (name == "Chest") {
                initialChests.push_back(glm::vec2(x, y));
            }
            else if (name == "Door") {
                initialDoors.push_back(std::make_pair(glm::vec3(x, y, z), w));
            }
            else if (name == "Lever") {
                initialLevers.push_back(std::make_pair(glm::vec3(x, y, z), w));
            }
            else if (name == "PressurePlate") {
                initialPressurePlates.push_back(std::make_pair(glm::vec3(x, y, z), w));
            }
            else if (name == "Light") {
                lights.push_back(new LightObject(glm::vec3(x, y, z)));
            }
        }
    }
    initPhysics();
    m_pWorld->addRigidBody(object->physicsObject->pRigidBody);
    resetLevel();
}