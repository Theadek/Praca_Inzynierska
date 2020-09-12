#pragma once
#include "Camera.h"
#include "ModelManager.h"
class Game
{
public:
    GLFWwindow* window = nullptr;
    float deltaTime = 0.0f;	
    float lastFrame = 0.0f;
    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;
    static float lastX;
    static float lastY;
    static bool firstMouse;
    static int DEBUG;
    static Camera* camera;
    std::vector <Model*> models;
    Game(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);
    ~Game();
    int init();
    void loadModels();
    void loadTextures();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void processInput(GLFWwindow* window);
};

