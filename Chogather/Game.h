#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <map>
#include "Game.h"
#include "Camera.h"
#include "ModelManager.h"
#include "Object.h"
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
    std::map <std::string, Model*> models;
    std::vector <Object*> objects;
    Game(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);
    ~Game();
    int init();
    void loadModels();
    void loadTextures();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void processInput(GLFWwindow* window);
};

