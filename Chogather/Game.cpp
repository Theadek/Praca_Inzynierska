#include "Game.h"

Camera* Game::camera;
float Game::lastX;
float Game::lastY;
bool Game::firstMouse;
int Game::DEBUG;

Game::Game(unsigned int WIDTH, unsigned int HEIGHT) {
    camera = new Camera(glm::vec3(0.0f, 0.5f, 12.0f));
    this->SCR_HEIGHT = HEIGHT;
    this->SCR_WIDTH = WIDTH;
    this->firstMouse = true;
    this->lastX = SCR_WIDTH / 2;
    this->lastY = SCR_HEIGHT / 2;
    this->DEBUG = 1;
    this->player = new Hero();
}

Game::~Game() {
    glfwTerminate();
}



void Game::loadModels() {
    //loadModel("cube", "Models/cube/cube.obj");
    //loadModel("nanosuit", "Models/nanosuit/nanosuit.obj");
    //loadModel("ball", "Models/ball/ball.obj");
    //loadModel("backpack", "Models/backpack/backpack.obj");

    //Model backpack(FileSystem::getPath("Models/backpack/backpack.obj"));
    Model backpack("Models/backpack/backpack.obj");
    models.insert({"backpack", backpack});
}

void Game::loadTextures() {
}

void Game::loadShaders() {
    Shader* lightShader = new Shader("Shaders/light.vert", "Shaders/light.frag");
    shaders.insert({ "lightShader", lightShader });
    Shader* objectShader = new Shader("Shaders/object.vert", "Shaders/object.frag");
    shaders.insert({ "objectShader", objectShader });
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
    loadModels();
    loadShaders();

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
    if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS)
        player->Move(JUMP, deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS)
        player->Move(CROUCH, deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        player->Move(RIGHT_MOVE, deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS)
        player->Move(LEFT_MOVE, deltaTime);
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
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

bool Game::detectCollision(Object* hero, Object* terrain) {

    //float additionalShiftX = 0, additionalShiftY = 0;
    //if (terrain->scale.y > 1.0f) {
    //    additionalShiftY = terrain->scale.y * terrain->model->size.y / 3;
    //}
    //if (terrain->scale.x > 1.0f) {
    //    additionalShiftX = terrain->scale.x * terrain->model->size.x / 3;
    //}
    //bool collisionX = (hero->position.x + (hero->scale.x * hero->model->size.x)) >= terrain->position.x + additionalShiftX &&
    //    (terrain->position.x + additionalShiftX +(terrain->scale.x * terrain->model->size.x)) >= hero->position.x;

    //bool collisionY = (hero->position.y + (hero->scale.y * hero->model->size.y)) >= terrain->position.y - additionalShiftY &&
    //    (terrain->position.y - additionalShiftY + (terrain->scale.y * terrain->model->size.y)) >= hero->position.y;

    //return collisionX && collisionY;
    return true;
}
