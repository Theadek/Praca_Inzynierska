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
    static int DEBUG;
    static Camera* camera;
    std::map <std::string, Model*> models;
    std::vector <Object*> objects;
    std::map <std::string, Shader*> shaders;
    Game(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);
    ~Game();
    int init();
    void loadModels();
    void loadTextures();
    void loadShaders();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void processInput(GLFWwindow* window);
    bool detectCollision(Object* hero, Object* terrain);
};

