#include "Game.h"

Camera* Game::camera;
Background* Game::background;
float Game::lastX;
float Game::lastY;
bool Game::firstMouse;
bool Game::Debug;

Game::Game(unsigned int WIDTH, unsigned int HEIGHT) {
    camera = new Camera(glm::vec3(0.0f, 0.5f, 12.0f));
    SCR_HEIGHT = HEIGHT;
    SCR_WIDTH = WIDTH;
    firstMouse = true;
    Debug = false;
    lastX = SCR_WIDTH / 2;
    lastY = SCR_HEIGHT / 2;
}

Game::~Game() {
    glfwTerminate();
}



void Game::loadModels() {

    Model level1("Models/Level1/Level1.obj", true);
    models.insert({ "level1", level1 });
}

void Game::loadTextures() {

}

void Game::loadShaders() {
    Shader* lightShader = new Shader("Shaders/light.vert", "Shaders/light.frag");
    shaders.insert({ "lightShader", lightShader });
    Shader* objectShader = new Shader("Shaders/object.vert", "Shaders/object.frag");
    shaders.insert({ "objectShader", objectShader });
    Shader* fontShader = new Shader("Shaders/Glyph.vert", "Shaders/Glyph.frag");
    shaders.insert({ "fontShader", fontShader });
}

void Game::loadObjects() {


    //level1 ----------------------------------------------------------------
    GraphicsObject* levelG = new GraphicsObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), -90.0f, &models.find("level1")->second);
    PhysicsObject* levelP = new PhysicsObject(levelG, 0.0f);
    Object* level = new Object(levelG, levelP, PLATFORM);
    level->physicsObject->pRigidBody->setLinearFactor(btVector3(1.0f, 1.0f, 0.0f));
    level->physicsObject->pRigidBody->setFriction(1.0f);
    level->physicsObject->pRigidBody->setRestitution(0.0f);

    //-------------------------------------------------------------------

    Hero* hero = new Hero(glm::vec2(-5.0f, 7.0f), glm::vec3(0.15f, 0.15f, 0.15f));
    DoorObject* door = new DoorObject(glm::vec2(-1.7f, 7.0f), glm::vec3(0.25f, 0.25f, 0.25f), -90.0f);
    DoorObject* door2 = new DoorObject(glm::vec2(-1.2f, 10.0f), glm::vec3(0.25f, 0.25f, 0.25f), -90.0f);
    LeverObject* lever = new LeverObject(glm::vec2(-3.5f, 6.0f), glm::vec3(0.1f, 0.1f, 0.1f), -90.0f);
    PressurePlateObject* pressurePlate = new PressurePlateObject(glm::vec2(8.0f,  5.15f), glm::vec3(0.2f, 0.1f, 0.2f), 0.0f);
    pressurePlate->bind(door2);
    lever->bind(door);
    LightObject* light = new LightObject(glm::vec3(4.0f, 12.0f, 20.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f);
    ChestObject* chest = new ChestObject(glm::vec3(1.0f, 10.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), -90.0f);

    player = hero;
    objects.push_back(level);
    doors.push_back(door);
    doors.push_back(door2);
    levers.push_back(lever);
    pressurePlates.push_back(pressurePlate);
    chests.push_back(chest);
    lights.push_back(light);
    m_pWorld->addRigidBody(hero->object->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(door->object->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(door2->object->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(lever->object->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(chest->object->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(pressurePlate->object->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(light->object->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(level->physicsObject->pRigidBody);
}

void Game::loadFonts()
{
    Font* chinaTown = new Font("Fonts/ChinaTown.ttf");
    fonts.insert({ "ChinaTown", chinaTown });
}

void Game::createUIElements()
{
    Font* chinaTown = fonts.find("ChinaTown")->second;

    UIElement* logo = new UIText(20.0f, 20.0f, 1.0f, "ChoGather", chinaTown, glm::vec3(1.0f, 0.0f, 0.0f));
    UIelements.insert({ "logo", logo });
}

int Game::init() {
    // glfw: initialize and configure
   // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Chogather", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);
    glfwSetCursorPosCallback(window, &mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    stbi_set_flip_vertically_on_load(true);
    background = new Background();
    loadModels();
    loadShaders();
    initPhysics();
    loadObjects();
    loadFonts();
    createUIElements();
    camera->TeleportToPosition(player->object->graphicsObject->position);
    return 1;
}

void Game::toggleDebug() {
    static bool debugEnabled = false;
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
        if (!debugEnabled) {
            Debug = !Debug;
            debugEnabled = !debugEnabled;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE) {
        if (debugEnabled) {
            debugEnabled = !debugEnabled;
        }
    }
}

void Game::processInput()
{
    if (Debug) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)) {
        if (isOnTheGround(player->object)) {
            player->isLeftWallJumpAvailable = true;
            player->isRightWallJumpAvailable = true;
            player->move(JUMP);
        }
        else if (isOnTheWallLeft(player->object) && (player->isLeftWallJumpAvailable)) {
            player->isLeftWallJumpAvailable = false;
            player->isRightWallJumpAvailable = true;
            player->move(JUMP);
        }
        else if (isOnTheWallRight(player->object) && (player->isRightWallJumpAvailable)) {
            player->isRightWallJumpAvailable = false;
            player->isLeftWallJumpAvailable = true;
            player->move(JUMP);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        player->move(CROUCH);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        player->move(RIGHT_MOVE);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        player->move(LEFT_MOVE);


    //we have to change it ----------------------------------------------------------------
    //space handle
    static int oldState = GLFW_RELEASE;
    int newState = glfwGetKey(window, GLFW_KEY_SPACE);
    if (newState == GLFW_RELEASE && oldState == GLFW_PRESS) {
        if (isInTheDepth(player->object)) {
            player->move(INTERACTION);
        }
    }
    oldState = newState;
    //-------------------------------------------------------------------------------------

    toggleDebug();
}


void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Game::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (Debug) {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

void Game::initPhysics() {
    m_pBroadphase = new btDbvtBroadphase();
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
    m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
    m_pSolver = new btSequentialImpulseConstraintSolver;
    m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);
    m_pWorld->setGravity(btVector3(0.0, GRAVITY, 0.0));

    debugDrawerObject = new debugDrawer(camera, SCR_WIDTH, SCR_HEIGHT);
    m_pWorld->setDebugDrawer(debugDrawerObject);
}

bool Game::isOnTheGround(Object* object) {
    float positionX = object->graphicsObject->position.x;
    float positionY = object->graphicsObject->position.y;
    float positionZ = object->graphicsObject->position.z;
    float halfOfSizeWithScaleY = object->graphicsObject->model->size.y * object->graphicsObject->scale.y / 2;
    float halfOfSizeWithScaleX = object->graphicsObject->model->size.x * object->graphicsObject->scale.x / 2;
    btCollisionWorld::ClosestRayResultCallback rayCallbackCenter(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY - halfOfSizeWithScaleY - 0.1, positionZ));
    btCollisionWorld::ClosestRayResultCallback rayCallbackLeft(btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY - 0.1, positionZ));
    btCollisionWorld::ClosestRayResultCallback rayCallbackRight(btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY - 0.1, positionZ));

    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY - halfOfSizeWithScaleY - 0.1, positionZ), rayCallbackCenter);
    m_pWorld->rayTest(btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY - 0.1, positionZ), rayCallbackLeft);
    m_pWorld->rayTest(btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY - 0.1, positionZ), rayCallbackRight);

    return (rayCallbackCenter.hasHit() || rayCallbackLeft.hasHit() || rayCallbackRight.hasHit());
}

bool Game::isBelow(Object* object) {
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

bool Game::isOnTheWallLeft(Object* object) {
    float positionX = object->graphicsObject->position.x;
    float positionY = object->graphicsObject->position.y;
    float positionZ = object->graphicsObject->position.z;
    float halfOfSizeWithScaleX = object->graphicsObject->model->size.x * object->graphicsObject->scale.x / 2;
    btCollisionWorld::ClosestRayResultCallback rayCallbackLeft(btVector3(positionX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ));
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ), rayCallbackLeft);
    if (rayCallbackLeft.hasHit()) {
        return true;
    }
    return false;

}

bool Game::isOnTheWallRight(Object* object) {
    float positionX = object->graphicsObject->position.x;
    float positionY = object->graphicsObject->position.y;
    float positionZ = object->graphicsObject->position.z;
    float halfOfSizeWithScaleX = object->graphicsObject->model->size.x * object->graphicsObject->scale.x / 2;
    btCollisionWorld::ClosestRayResultCallback rayCallbackRight(btVector3(positionX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ));
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ), rayCallbackRight);
    if (rayCallbackRight.hasHit()) {
        return true;
    }
    return false;
}

bool Game::isInTheDepth(Object* object) {
    float positionX = object->graphicsObject->position.x;
    float positionY = object->graphicsObject->position.y;
    float positionZ = object->graphicsObject->position.z;
    btCollisionWorld::ClosestRayResultCallback rayCallbackCenter(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY, DEPTH_TEST_RANGE));
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY, DEPTH_TEST_RANGE), rayCallbackCenter);
    if (rayCallbackCenter.hasHit()) {
        //We have to store object position bcs we dont have our rayCast system, later we will check  if it is lever
        btTransform trans = rayCallbackCenter.m_collisionObject->getWorldTransform();
        player->leverPos = glm::vec2(trans.getOrigin().getX(), trans.getOrigin().getY());
        //cout << "Smth is in the depth" << endl;
        return true;
    }
    else {
        //cout << "Nothing is there!" << endl;
        return false;
    }
}

void Game::draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    background->draw();
    if (Debug)
        m_pWorld->debugDrawWorld();

    player->object->graphicsObject->renderModel(shaders.find("objectShader")->second);

    for (Object* object : objects) {
            object->graphicsObject->renderModel(shaders.find("objectShader")->second);
    }

    for (LeverObject* lever : levers) {
        lever->object->graphicsObject->renderModel(shaders.find("objectShader")->second);
    }

    for (PressurePlateObject* pressurePlate : pressurePlates) {
        pressurePlate->object->graphicsObject->renderModel(shaders.find("objectShader")->second);
    }

    for (DoorObject* door : doors) {
        door->object->graphicsObject->renderModel(shaders.find("objectShader")->second);
    }

    for (ChestObject* chest : chests) {
        chest->object->graphicsObject->renderModel(shaders.find("objectShader")->second);
    }

    for (LightObject* light : lights) {
        light->object->graphicsObject->renderModel(shaders.find("lightShader")->second);
    }

    for (std::pair<std::string, UIElement*> element : UIelements) {
        Shader* shader = shaders.find("fontShader")->second;
        element.second->draw(shader);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::update() {
    static float lastFrame = glfwGetTime();
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    processInput();
    m_pWorld->stepSimulation(deltaTime);

    player->update(isOnTheGround(player->object));

    for (LeverObject* lever : levers) {
        if (player->state == ACTION) {
            //it shouldn't work like this, but since we don't have our rayCast system, we have to use bullet
            if (lever->object->physicsObject->btPosition.getX() == player->leverPos.x && lever->object->physicsObject->btPosition.getY() == player->leverPos.y) {
                lever->pull();
                player->state = STAYING;
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

    for (LightObject* light : lights) {
        shaders.find("objectShader")->second->use();
        shaders.find("objectShader")->second->setVec3("lightPosition", light->object->graphicsObject->position);
    }

    if(!Debug)
        camera->WatchObject(player->object->graphicsObject->position, 0.9, deltaTime);


    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    //glm::mat4 view = glm::lookAt(glm::vec3(10, 6, 25), glm::vec3(5, 3, 0), glm::vec3(0.0f, 1.0f, 0.0f));
    shaders.find("objectShader")->second->use();
    shaders.find("objectShader")->second->setMat4("view", view);
    shaders.find("objectShader")->second->setMat4("projection", projection);
    shaders.find("objectShader")->second->setVec3("viewPos", camera->Position);

    shaders.find("lightShader")->second->use();
    shaders.find("lightShader")->second->setMat4("view", view);
    shaders.find("lightShader")->second->setMat4("projection", projection);

    glm::mat4 projectionUI = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    shaders.find("fontShader")->second->use();
    shaders.find("fontShader")->second->setMat4("projection", projectionUI);
}