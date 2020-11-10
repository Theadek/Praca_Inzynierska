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
    //loadModel("cube", "Models/cube/cube.obj");
    //loadModel("nanosuit", "Models/nanosuit/nanosuit.obj");
    //loadModel("ball", "Models/ball/ball.obj");
    //loadModel("backpack", "Models/backpack/backpack.obj");



    Model cube("Models/cube/cube.obj");
    models.insert({ "cube", cube });

    for (int i = 1; i <= JUMP_MODELS; i++) {
        string jumpModelName = (i < 10) ? "Models/Jump/jump_00000" + to_string(i) + ".obj" : "Models/Jump/jump_0000" + to_string(i) + ".obj";
        Model jump(jumpModelName);
        models.insert({ "jump" + to_string(i), jump });
    }

    for (int i = 1; i <= WALK_MODELS; i++) {
        string walkModelName = (i < 10) ? "Models/Walk/walk_00000" + to_string(i) + ".obj" : "Models/Walk/walk_0000" + to_string(i) + ".obj";
        Model walk(walkModelName);
        models.insert({ "walk"+to_string(i), walk });
    }
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
    GraphicsObject* graphicsObject = new GraphicsObject(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f), 90.0f, &models.find("walk1")->second);
    PhysicsObject* physicsObject = new PhysicsObject(graphicsObject, 1.0f);
    Object* cube = new Object(graphicsObject, physicsObject, HERO);

    for (int i = 1; i <= JUMP_MODELS; i++) {
        string jumpModelName = "jump" + to_string(i);
        graphicsObject->modelsForJumpAnimation.push_back(&models.find(jumpModelName)->second);
    }

    for (int i = 1; i <= WALK_MODELS; i++) {
        string walkModelName = "walk" + to_string(i);
        graphicsObject->modelsForWalkAnimation.push_back(&models.find(walkModelName)->second);
    }

    cube->physicsObject->pRigidBody->setLinearFactor(btVector3(1.0f, 1.0f, 1.0f));
    cube->physicsObject->pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
    cube->physicsObject->pRigidBody->setFriction(1.0f);
    cube->physicsObject->pRigidBody->setRestitution(0.0f);

    GraphicsObject* graphicsObject2 = new GraphicsObject(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(10.0f, 1.0f, 1.0f), 0.0f, &models.find("cube")->second);
    PhysicsObject* physicsObject2 = new PhysicsObject(graphicsObject2, 0.0f);
    Object* floor2 = new Object(graphicsObject2, physicsObject2, PLATFORM);
    floor2->physicsObject->pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
    floor2->physicsObject->pRigidBody->setFriction(1.0f);
    floor2->physicsObject->pRigidBody->setRestitution(0.0f);

    GraphicsObject* graphicsObject3 = new GraphicsObject(glm::vec3(3.0f, 1.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f), 0.0f, &models.find("cube")->second);
    PhysicsObject* physicsObject3 = new PhysicsObject(graphicsObject3, 0.0f);
    Object* floor3 = new Object(graphicsObject3, physicsObject3, PLATFORM);
    floor3->physicsObject->pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
    floor3->physicsObject->pRigidBody->setFriction(1.0f);
    floor3->physicsObject->pRigidBody->setRestitution(0.0f);

    GraphicsObject* lightGraphicsObject = new GraphicsObject(glm::vec3(4.0f, 6.0f, 6.0f), glm::vec3(1.5f, 1.5f, 1.5f), 0.0f, &models.find("cube")->second);
    PhysicsObject* lightPhysicsObject = new PhysicsObject(lightGraphicsObject, 0.0f);
    Object* light = new Object(lightGraphicsObject, lightPhysicsObject, LIGHT);
    light->physicsObject->pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
    light->physicsObject->pRigidBody->setFriction(1.0f);
    light->physicsObject->pRigidBody->setRestitution(0.0f);

    Hero* hero = new Hero(cube);
    player = hero;
    objects.push_back(cube);
    objects.push_back(floor2);
    objects.push_back(floor3);
    objects.push_back(light);
    m_pWorld->addRigidBody(cube->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(floor2->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(floor3->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(light->physicsObject->pRigidBody);
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
    if (((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)) &&
        isOnTheGround(player->hero))
        player->Move(JUMP);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        player->Move(CROUCH);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        player->Move(RIGHT_MOVE);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        player->Move(LEFT_MOVE);
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
    float halfOfSizeWithScaleX = object->graphicsObject->model->size.x * object->graphicsObject->scale.x / 2;
    float halfOfSizeWithScaleY = object->graphicsObject->model->size.y * object->graphicsObject->scale.y / 2;
    btCollisionWorld::ClosestRayResultCallback rayCallbackCenter(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY - halfOfSizeWithScaleY, positionZ));
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY - halfOfSizeWithScaleY, positionZ), rayCallbackCenter);
    if (rayCallbackCenter.hasHit()) {
        return true;
    }
    else {
        return false;
    }
}

void Game::draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    background->draw();
    if (Debug)
        m_pWorld->debugDrawWorld();
    for (Object* object : objects) {
        if (object->tag == LIGHT) {
            object->graphicsObject->renderModel(shaders.find("lightShader")->second);
        }
        else {
            object->graphicsObject->renderModel(shaders.find("objectShader")->second);
        }
    }
    for (std::pair<std::string, UIElement*> element : UIelements) {
        Shader* shader = shaders.find("fontShader")->second;
        element.second->draw(shader);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::update() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    processInput();
    m_pWorld->stepSimulation(deltaTime);
    for (Object* object : objects) {
        btTransform trans = object->physicsObject->pRigidBody->getWorldTransform();
        object->graphicsObject->position.x = trans.getOrigin().x();
        object->graphicsObject->position.y = trans.getOrigin().y();
        object->physicsObject->pRigidBody->setActivationState(ACTIVE_TAG);
        if (object->tag == LIGHT) {
            shaders.find("objectShader")->second->use();
            shaders.find("objectShader")->second->setVec3("lightPosition", object->graphicsObject->position);
        }
        if (object->tag == HERO) {
            if (player->isFacingRight) {
                player->hero->graphicsObject->rotate = 90.0f;
            }
            else {
                player->hero->graphicsObject->rotate = -90.0f;
            }
            if (isOnTheGround(object)) {
                player->hero->graphicsObject->isJumping = false;
            }
            else {
                player->hero->graphicsObject->isJumping = true;
            }
            if (object->physicsObject->pRigidBody->getLinearVelocity().getX() != 0) {
                object->graphicsObject->isWalking = true;
            }
            else {
                object->graphicsObject->isWalking = false;
            }
        }
    }

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