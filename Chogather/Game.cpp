#include "Game.h"

Camera* Game::camera;
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
    this->player = new Hero();
    //player->currentJump = JUMP_HEIGHT;
    player->currentState = WALKING;
    player->gravityCounter = JUMP_HEIGHT;
}

Game::~Game() {
    glfwTerminate();
}

void Game::loadModels() {
    Model* cube = new Model();
    cube->loadobj("Models/cube/cube.obj");
    cube->name = "cube";
    models.insert({ "cube", cube });

   /* Model* backpack = new Model();
    backpack->loadobj("Models/backpack/backpack.obj");
    backpack->name = "backpack";
    models.insert({ "backpack", backpack });*/

    /*Model* ball = new Model();
    ball->loadobj("Models/ball/ball.obj");
    ball->name = "ball";
    models.insert({ "ball", ball });*/

    Model* nanosuit = new Model();
    nanosuit->loadobj("Models/nanosuit/nanosuit.obj");
    nanosuit->name = "nanosuit";
    models.insert({ "nanosuit", nanosuit });
}

void Game::loadTextures() {
}

void Game::loadShaders() {
    Shader* lightShader = new Shader("Shaders/light.vs", "Shaders/light.fs");
    shaders.insert({ "lightShader", lightShader });
    Shader* basicShader = new Shader("Shaders/vertexShader.vs", "Shaders/fragmentShader.fs");
    shaders.insert({ "basicShader", basicShader });
    Shader* collisionBoxShader = new Shader("Shaders/collisionBox.vs", "Shaders/collisionBox.fs");
    shaders.insert({ "collisionBoxShader", collisionBoxShader });
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
    loadModels();
    loadShaders();
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
    if ((glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS) && (isColliding() || player->hero->position.y <= 0))
        player->Move(JUMP, deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS)
        player->Move(CROUCH, deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        player->Move(RIGHT_MOVE, deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS)
        player->Move(LEFT_MOVE, deltaTime);

    updatePosition();
}


void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Game::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
}

bool Game::detectCollision(Object* firstObject, Object* secondObject) {

    glm::vec3 updatedSecondObjectCorner = secondObject->position + secondObject->model->corner;
    glm::vec3 updatedFirstObjectCorner = firstObject->position + firstObject->model->corner;

    bool collisionX = (updatedFirstObjectCorner.x + (firstObject->scale.x * firstObject->model->size.x)) >= updatedSecondObjectCorner.x  &&
        (updatedSecondObjectCorner.x +(secondObject->scale.x * secondObject->model->size.x)) >= updatedFirstObjectCorner.x;

    bool collisionY = (updatedFirstObjectCorner.y + (firstObject->scale.y * firstObject->model->size.y)) >= updatedSecondObjectCorner.y  &&
        (updatedSecondObjectCorner.y + (secondObject->scale.y * secondObject->model->size.y)) >= updatedFirstObjectCorner.y;

    bool collisionZ = (updatedFirstObjectCorner.z + (firstObject->scale.z * firstObject->model->size.z)) >= updatedSecondObjectCorner.z  &&
        (updatedSecondObjectCorner.z  + (secondObject->scale.z * secondObject->model->size.z)) >= updatedFirstObjectCorner.z;

    return collisionX && collisionY && collisionZ;
}

bool Game::isColliding() {

    glm::vec3 newHeroCorner = player->hero->position + player->hero->model->corner;


    for (Object* terrain : objects) {
        glm::vec3 newTerrainCorner = terrain->position + terrain->model->corner * terrain->scale;

        bool collisionX = (newHeroCorner.x + (player->hero->scale.x * player->hero->model->size.x)) >= newTerrainCorner.x &&
            (newTerrainCorner.x + (terrain->scale.x * terrain->model->size.x)) >= newHeroCorner.x;

        bool collisionY = (newHeroCorner.y + (player->hero->scale.y * player->hero->model->size.y)) >= newTerrainCorner.y&&
            (newTerrainCorner.y + (terrain->scale.y * terrain->model->size.y)) >= newHeroCorner.y;

        bool collisionZ = (newHeroCorner.z + (player->hero->scale.z * player->hero->model->size.z)) >= newTerrainCorner.z &&
            (newTerrainCorner.z + (terrain->scale.z * terrain->model->size.z)) >= newHeroCorner.z;

        if (collisionX && collisionY && collisionZ)
        {
            return true;
        }
    }
    return false;
}

Object* Game::isCollidingOnNextMove(char axis) {

    glm::vec3 newHeroCorner = player->hero->position + player->hero->model->corner;
    if (axis == 'x') {
        newHeroCorner.x += player->shiftVector.x;
    }
    else if (axis == 'y') {
        newHeroCorner.y += player->shiftVector.y;
    }
    else if (axis == 'z') {
        newHeroCorner.z += player->shiftVector.z;
    }

    for (Object* terrain : objects) {
        glm::vec3 newTerrainCorner = terrain->position + terrain->model->corner * terrain->scale;

        bool collisionX = (newHeroCorner.x + (player->hero->scale.x * player->hero->model->size.x)) >= newTerrainCorner.x &&
            (newTerrainCorner.x + (terrain->scale.x * terrain->model->size.x)) >= newHeroCorner.x;

        bool collisionY = (newHeroCorner.y + (player->hero->scale.y * player->hero->model->size.y)) >= newTerrainCorner.y &&
            (newTerrainCorner.y + (terrain->scale.y * terrain->model->size.y)) >= newHeroCorner.y;

        bool collisionZ = (newHeroCorner.z + (player->hero->scale.z * player->hero->model->size.z)) >= newTerrainCorner.z &&
            (newTerrainCorner.z + (terrain->scale.z * terrain->model->size.z)) >= newHeroCorner.z;

        if (collisionX && collisionY && collisionZ)
        {
            return terrain;
        }
    }
    return NULL;
}


void Game::updatePosition() {


    switch (player->currentState) {
    case WALKING:
        //player->shiftVector.y -= GRAVITY * deltaTime * player->speed;
        break;
    case JUMPING:
        player->gravityCounter -= 0.1f;
        player->shiftVector.y += GRAVITY * player->gravityCounter * deltaTime * player->speed;
        if (player->gravityCounter < 0) {
            player->currentState = FALLING_DOWN;
        }
        break;
    case FALLING_DOWN:
        player->gravityCounter += 0.1f;
        player->shiftVector.y -= GRAVITY * player->gravityCounter * deltaTime * player->speed;
        if (player->gravityCounter >= JUMP_HEIGHT) {
            player->gravityCounter = JUMP_HEIGHT;
        }
        break;
    }

    //update position

    Object* terrain = isCollidingOnNextMove('x');
    if(terrain == NULL) {
        player->hero->position.x += player->shiftVector.x;
    }
    else {
        fixedUpdatePosition(terrain, 'x');
    }
    terrain = isCollidingOnNextMove('y');
    if (terrain == NULL) {
        player->hero->position.y += player->shiftVector.y;
        if (player->hero->position.y <= 0) {
            player->hero->position.y = 0;
        }
    } else {
        fixedUpdatePosition(terrain, 'y');
    }

    player->shiftVector = glm::vec3(0.0f, 0.0f, 0.0f);

    //update states

    if (player->currentState == JUMPING && isColliding()) {
        player->currentState = FALLING_DOWN;
    }
    else if (player->currentState == WALKING && !isColliding()) {
        player->currentState = FALLING_DOWN;
    }
    else if (player->currentState == FALLING_DOWN && isColliding()) {
        player->currentState = WALKING;
    }
    else if (player->hero->position.y <= 0) {
        player->currentState = WALKING;
    }

}

void Game::fixedUpdatePosition(Object* terrain, char axis) {

    glm::vec3 newTerrainCorner = terrain->position + terrain->model->corner;
    glm::vec3 newHeroCorner = player->hero->position + player->hero->model->corner;

    if (axis == 'x') {
        if (newHeroCorner.x < newTerrainCorner.x) {
            //TODO
        }
        else {
            // TODO
        }
    }

    if (axis == 'y') {
        if (newHeroCorner.y > newTerrainCorner.y) {
            player->hero->position.y = -player->hero->model->corner.y + terrain->position.y + (terrain->model->corner.y * terrain->scale.y) + (terrain->scale.y * terrain->model->size.y);
        }
        else {
            player->hero->position.y = -player->hero->model->corner.y + terrain->position.y + (terrain->model->corner.y * terrain->scale.y) - (player->hero->scale.y * player->hero->model->size.y);
        }
    }
}
