#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <map>
#include "Game.h"
#include "Camera.h"
#include "Model.h"
#include "Object.h"
#include "Hero.h"
#include "Shader.h"
#include "Font.h"
#include "UIElement.h"
#include "UIText.h"
#include "stb_image.h"
#include "debugDrawer.h"
#include "Background.h"
#include "AnimatedGraphicsObject.h"
#include "DoorObject.h"
#include "LeverObject.h"
#include "PressurePlateObject.h"
#include "ChestObject.h"
#include "LightObject.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>


#define GRAVITY -20
#define DEPTH_TEST_RANGE -5.0f

class Game
{
public:
    GLFWwindow* window = nullptr;
    Hero* player;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;
    static float lastX;
    static float lastY;
    static bool firstMouse;
    static bool Debug;
    static Camera* camera;
    static Background* background;
    std::map <std::string, Model> models;
    std::vector <Object*> objects;
    std::vector <LeverObject*> levers;
    std::vector <DoorObject*> doors;
    std::vector <PressurePlateObject*> pressurePlates;
    std::vector <ChestObject*> chests;
    std::vector <LightObject*> lights;
    std::map <std::string, Shader*> shaders;
    std::map <std::string, Font* > fonts;
    std::map <std::string, UIElement* > UIelements;
    //physics
    btBroadphaseInterface* m_pBroadphase;
    btDefaultCollisionConfiguration* m_pCollisionConfiguration;
    btCollisionDispatcher* m_pDispatcher;
    btConstraintSolver* m_pSolver;
    btDynamicsWorld* m_pWorld;
    btIDebugDraw* debugDrawerObject;
    //end of physics
    Game(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);
    ~Game();
    int init();
    void loadModels();
    void loadTextures();
    void loadShaders();
    void loadObjects();
    void loadFonts();
    void createUIElements();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void processInput();
    void initPhysics();
    bool isOnTheGround(Object* object);
    bool isBelow(Object* object);
    bool isOnTheWallLeft(Object* object);
    bool isOnTheWallRight(Object* object);
    bool isInTheDepth(Object* object);
    void draw();
    void update();

private:
    void toggleDebug();
};

