#pragma once
#include <glad/glad.h>
#include <Windows.h>
#include <gl/GLU.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <map>
#include "Model.h"
#include "Object.h"
#include "Hero.h"
#include "Shader.h"
#include "Font.h"
#include "UIElement.h"
#include "UIText.h"
#include "stb_image.h"
#include "Background.h"
#include "AnimatedGraphicsObject.h"
#include "SingleplayerLevelObject.h"
#include "MultiplayerLevelObject.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

enum GameState {
    GameState_PLAYERS,
    GameState_MENU,
    GameState_GAME
};

enum Key {
    Key_SPACE,
    Key_UP,
    Key_Q,
    Key_W,
    Key_S,
    Key_DOWN,
    Key_ENTER,
    Key_ESCAPE,
    Key_1,
    Key_2,
    Key_3,
    Key_NOTHING
};

class Game
{
public:
    GLFWwindow* window = nullptr;
    Hero* player;
    Hero* player2;
    float deltaTime = 0.0f;
    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;
    static float lastX;
    static float lastY;
    static bool firstMouse;
    static bool Debug;
    static Camera* camera;
    static Background* background;
    GameState actualState;
    static LevelObject* actualLevel;
    static int menuChoice;
    static Key actualKey;
    static bool multiplayer;
    std::map <std::string, Model> models;
    std::vector <SingleplayerLevelObject*> singleplayerLevels;
    std::vector <MultiplayerLevelObject*> multiplayerLevels;
    std::map <std::string, Shader*> shaders;
    std::map <std::string, Font* > fonts;
    std::map <std::string, UIElement* > UIelements;
    std::map <std::string, UIElement* > startMenuUIelements;
    std::map <std::string, UIElement* > singleplayerMenuUIelements;
    std::map <std::string, UIElement* > multiplayerMenuUIelements;
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
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void processInput();
    void draw();
    void update();

private:
    void toggleDebug();
};

