#include "Game.h"

Camera* Game::camera;
Background* Game::background;
float Game::lastX;
float Game::lastY;
bool Game::firstMouse;

Game::Game(unsigned int WIDTH, unsigned int HEIGHT) {
    camera = new Camera(glm::vec3(0.0f, 0.5f, 12.0f));
    this->SCR_HEIGHT = HEIGHT;
    this->SCR_WIDTH = WIDTH;
    this->firstMouse = true;
    this->lastX = SCR_WIDTH / 2;
    this->lastY = SCR_HEIGHT / 2;
}

Game::~Game() {
    glfwTerminate();
}



void Game::loadModels() {
    //loadModel("cube", "Models/cube/cube.obj");
    //loadModel("nanosuit", "Models/nanosuit/nanosuit.obj");
    //loadModel("ball", "Models/ball/ball.obj");
    //loadModel("backpack", "Models/backpack/backpack.obj");


   /* Model backpack("Models/backpack/backpack.obj");
    models.insert({"backpack", backpack});*/

     Model cube("Models/cube/cube.obj");
     models.insert({ "cube", cube });

}

void Game::loadTextures() {

}

void Game::loadShaders() {
    Shader* lightShader = new Shader("Shaders/light.vert", "Shaders/light.frag");
    shaders.insert({ "lightShader", lightShader });
    Shader* objectShader = new Shader("Shaders/object.vert", "Shaders/object.frag");
    shaders.insert({ "objectShader", objectShader });
}

void Game::loadObjects() {
    GraphicsObject* graphicsObject = new GraphicsObject(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, &models.find("cube")->second);
    PhysicsObject* physicsObject = new PhysicsObject(graphicsObject, 1.0f);
    Object* cube = new Object(graphicsObject, physicsObject, HERO);
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

    GraphicsObject* lightGraphicsObject = new GraphicsObject(glm::vec3(4.0f, 4.0f, 2.0f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, &models.find("cube")->second);
    PhysicsObject* lightPhysicsObject = new PhysicsObject(lightGraphicsObject, 0.0f);
    Object* light = new Object(lightGraphicsObject, lightPhysicsObject, LIGHT);
    light->physicsObject->pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
    light->physicsObject->pRigidBody->setFriction(1.0f);
    light->physicsObject->pRigidBody->setRestitution(0.0f);

    Hero* hero = new Hero(cube);
    player = hero;
    objects.push_back(cube);
    objects.push_back(floor2);
    objects.push_back(light);
    m_pWorld->addRigidBody(cube->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(floor2->physicsObject->pRigidBody);
    m_pWorld->addRigidBody(light->physicsObject->pRigidBody);
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
    this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Chogather", NULL, NULL);
    if (this->window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(this->window);
    glfwSetFramebufferSizeCallback(this->window, &framebuffer_size_callback);
    glfwSetCursorPosCallback(window, &mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    stbi_set_flip_vertically_on_load(true);
    background = new Background();
    loadModels();
    loadShaders();
    initPhysics();
    loadObjects();
    return 1;
}

void Game::processInput(GLFWwindow* window)
{
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);
    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
    if ((glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS) && isOnTheGround(player->hero))
        player->Move(JUMP);
    if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS)
        player->Move(CROUCH);
    if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        player->Move(RIGHT_MOVE);
    if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS)
        player->Move(LEFT_MOVE);
}


void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Game::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (DEBUG) {
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
    btCollisionWorld::ClosestRayResultCallback rayCallbackLeft(btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY, positionZ));
    btCollisionWorld::ClosestRayResultCallback rayCallbackRight(btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY, positionZ));
    btCollisionWorld::ClosestRayResultCallback rayCallbackCenter(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY - halfOfSizeWithScaleY, positionZ));
    m_pWorld->rayTest(btVector3(positionX - halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX - halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY, positionZ), rayCallbackLeft);
    m_pWorld->rayTest(btVector3(positionX + halfOfSizeWithScaleX, positionY, positionZ), btVector3(positionX + halfOfSizeWithScaleX, positionY - halfOfSizeWithScaleY, positionZ), rayCallbackRight);
    m_pWorld->rayTest(btVector3(positionX, positionY, positionZ), btVector3(positionX, positionY - halfOfSizeWithScaleY, positionZ), rayCallbackCenter);
    if (rayCallbackLeft.hasHit() || rayCallbackRight.hasHit() || rayCallbackCenter.hasHit()) {
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
    if (DEBUG)
        m_pWorld->debugDrawWorld();
    for (Object* object : objects) {
        if (object->tag == LIGHT) {
            object->graphicsObject->renderModel(shaders.find("lightShader")->second);
        }
        else {
            object->graphicsObject->renderModel(shaders.find("objectShader")->second);
        }
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::update() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    processInput(window);
    m_pWorld->stepSimulation(deltaTime);
    for (Object* object : objects) {
        btTransform trans = object->physicsObject->pRigidBody->getWorldTransform();
        object->graphicsObject->position.x = trans.getOrigin().x();
        object->graphicsObject->position.y = trans.getOrigin().y();
        object->physicsObject->pRigidBody->setActivationState(ACTIVE_TAG);
        if(object->tag == LIGHT)
            shaders.find("objectShader")->second->setVec3("lightPosition", object->graphicsObject->position);
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
}