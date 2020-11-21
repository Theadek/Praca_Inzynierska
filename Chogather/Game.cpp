#include "Game.h"

Camera* Game::camera;
Background* Game::background;
float Game::lastX;
float Game::lastY;
bool Game::firstMouse;
bool Game::Debug;
int Game::menuChoice;
LevelObject* Game::actualLevel;
bool Game::multiplayer;
Key Game::actualKey;
Game::Game(unsigned int WIDTH, unsigned int HEIGHT) {
    camera = new Camera(glm::vec3(0.0f, 0.5f, 12.0f));
    SCR_HEIGHT = HEIGHT;
    SCR_WIDTH = WIDTH;
    firstMouse = true;
    Debug = false;
    lastX = SCR_WIDTH / 2;
    lastY = SCR_HEIGHT / 2;
    actualState = GameState_PLAYERS;
    menuChoice = 0;
    actualKey = Key_NOTHING;
    multiplayer = false;
}


Game::~Game() {
    glfwTerminate();
}


void Game::loadModels() {

    Model level1("Models/Level1/Level1Test.obj", true);
    models.insert({ "level1", level1 });

    Model level2("Models/Level2/Level2.obj", true);
    models.insert({ "level2", level2 });
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

    Hero* playerSingleplayerLevel1 = new Hero(glm::vec2(-5.0f, 7.0f));
    Hero* playerSingleplayerLevel2 = new Hero(glm::vec2(-5.0f, 7.0f));
    Hero* player1MultiplayerLevel1 = new Hero(glm::vec2(-5.0f, 13.0f));
    Hero* player2MultiplayerLevel1 = new Hero(glm::vec2(0.0f, 16.0f));
    DoorObject* door = new DoorObject(glm::vec2(-1.7f, 7.0f));
    DoorObject* door2 = new DoorObject(glm::vec2(-1.2f, 10.0f));
    DoorObject* door3 = new DoorObject(glm::vec2(-1.2f, 10.0f));
    LeverObject* lever = new LeverObject(glm::vec2(-3.5f, 6.0f));
    PressurePlateObject* pressurePlate = new PressurePlateObject(glm::vec2(8.0f, 5.15f));
    lever->bind(door);
    pressurePlate->bind(door2);
    LightObject* light = new LightObject(glm::vec3(4.0f, 12.0f, 50.0f));
    LightObject* light2 = new LightObject(glm::vec3(4.0f, 12.0f, 50.0f));
    LightObject* light3 = new LightObject(glm::vec3(4.0f, 12.0f, 50.0f));
    ChestObject* chest = new ChestObject(glm::vec3(1.0f, 10.0f, 0.0f));


    //first singleplayerLevel
    SingleplayerLevelObject* singleplayerLevel1 = new SingleplayerLevelObject(&models.find("level1")->second, glm::vec2(-3.0f,11.0f), camera, SCR_WIDTH, SCR_HEIGHT);
    singleplayerLevel1->doors.push_back(door);
    singleplayerLevel1->levers.push_back(lever);
    singleplayerLevel1->pressurePlates.push_back(pressurePlate);
    singleplayerLevel1->chests.push_back(chest);
    singleplayerLevel1->lights.push_back(light);
    singleplayerLevel1->doors.push_back(door2);
    singleplayerLevel1->hero = playerSingleplayerLevel1;
    singleplayerLevel1->loadLevel();
    singleplayerLevels.push_back(singleplayerLevel1);

    //second singleplayerLevel
    SingleplayerLevelObject* singleplayerLevel2 = new SingleplayerLevelObject(&models.find("level2")->second, glm::vec2(-1.0f, 9.0f), camera, SCR_WIDTH, SCR_HEIGHT);
    singleplayerLevel2->hero = playerSingleplayerLevel2;
    singleplayerLevel2->lights.push_back(light2);
    singleplayerLevel2->loadLevel();
    singleplayerLevels.push_back(singleplayerLevel2);

    //first multiplayerLevel
    MultiplayerLevelObject* multiplayerLevel1 = new MultiplayerLevelObject(&models.find("level2")->second, glm::vec2(-5.0f, 7.0f), camera, SCR_WIDTH, SCR_HEIGHT);
    multiplayerLevel1->doors.push_back(door3);
    multiplayerLevel1->hero = player1MultiplayerLevel1;
    multiplayerLevel1->hero2 = player2MultiplayerLevel1;
    multiplayerLevel1->lights.push_back(light3);
    multiplayerLevel1->loadLevel();
    multiplayerLevels.push_back(multiplayerLevel1);
}

void Game::loadFonts()
{
    Font* chinaTown = new Font("Fonts/ChinaTown.ttf");
    fonts.insert({ "ChinaTown", chinaTown });
}

void Game::createUIElements()
{
    Font* chinaTown = fonts.find("ChinaTown")->second;

    float textSize = chinaTown->sizeOfText("ChoGather", 3.0f);
    UIElement* menuLogo = new UIText(((SCR_WIDTH/2) - textSize /2), SCR_HEIGHT * 0.75, 3.0f, "ChoGhater", chinaTown, glm::vec3(0.4f, 0.0f, 0.8f));
    startMenuUIelements.insert({ "menuLogo", menuLogo });

    std::string numberOfPlayers1 = "1 Player";
    textSize = chinaTown->sizeOfText(numberOfPlayers1, 1.5f);
    UIElement* numberOfPlayers1Text = new UIText(((SCR_WIDTH / 2) - textSize / 2), SCR_HEIGHT * 0.5, 1.5f, numberOfPlayers1, chinaTown, glm::vec3(0.4f, 0.0f, 0.8f));
    startMenuUIelements.insert({ numberOfPlayers1, numberOfPlayers1Text });

    std::string numberOfPlayers2 = "2 Players";
    textSize = chinaTown->sizeOfText(numberOfPlayers2, 1.5f);
    UIElement* numberOfPlayers2Text = new UIText(((SCR_WIDTH / 2) - textSize / 2), SCR_HEIGHT * 0.4, 1.5f, numberOfPlayers2, chinaTown, glm::vec3(0.4f, 0.0f, 0.8f));
    startMenuUIelements.insert({ numberOfPlayers2, numberOfPlayers2Text });

    for (int i = 1; i <= singleplayerLevels.size(); i++) {
        std::string levelName = "Level " + to_string(i);
        float textSize = chinaTown->sizeOfText(levelName, 1.5f);
        float scaleDown = 0.6 - float(i) / 10;
        UIElement* levelText = new UIText(((SCR_WIDTH / 2) - textSize / 2), SCR_HEIGHT * scaleDown, 1.5f, levelName, chinaTown, glm::vec3(0.4f, 0.0f, 0.8f));
        singleplayerMenuUIelements.insert({ levelName, levelText });
    }

    for (int i = 1; i <= multiplayerLevels.size(); i++) {
        std::string levelName = "Level " + to_string(i);
        float textSize = chinaTown->sizeOfText(levelName, 1.5f);
        float scaleDown = 0.6 - float(i) / 10;
        UIElement* levelText = new UIText(((SCR_WIDTH / 2) - textSize / 2), SCR_HEIGHT * scaleDown, 1.5f, levelName, chinaTown, glm::vec3(0.4f, 0.0f, 0.8f));
        multiplayerMenuUIelements.insert({ levelName, levelText });
    }



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
    glfwSetKeyCallback(window, &key_callback);
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
    //GAME
    if (actualState == GameState_GAME) {
        if (Debug) {
            if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
                camera->ProcessKeyboard(FORWARD, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
                camera->ProcessKeyboard(BACKWARD, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
                camera->ProcessKeyboard(LEFT, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
                camera->ProcessKeyboard(RIGHT, deltaTime);
        }
        //first player move
        if (actualKey == Key_UP) {
            if (actualLevel->isOnTheGround(player)) {
                player->isLeftWallJumpAvailable = true;
                player->isRightWallJumpAvailable = true;
                player->move(JUMP);
            }
            else if (actualLevel->isOnTheWallLeft(player) && (player->isLeftWallJumpAvailable)) {
                player->isLeftWallJumpAvailable = false;
                player->isRightWallJumpAvailable = true;
                player->move(JUMP);
            }
            else if (actualLevel->isOnTheWallRight(player) && (player->isRightWallJumpAvailable)) {
                player->isRightWallJumpAvailable = false;
                player->isLeftWallJumpAvailable = true;
                player->move(JUMP);
            }
        }
        if (actualKey == Key_SPACE) {
            if (actualLevel->isInTheDepth(player)) {
                player->move(INTERACTION);
            }
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            player->move(RIGHT_MOVE);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            player->move(LEFT_MOVE);
        //second player move
        if (player2 != NULL) {
            if (actualKey == Key_W) {
                if (actualLevel->isOnTheGround(player2)) {
                    player2->isLeftWallJumpAvailable = true;
                    player2->isRightWallJumpAvailable = true;
                    player2->move(JUMP);
                }
                else if (actualLevel->isOnTheWallLeft(player2) && (player2->isLeftWallJumpAvailable)) {
                    player2->isLeftWallJumpAvailable = false;
                    player2->isRightWallJumpAvailable = true;
                    player2->move(JUMP);
                }
                else if (actualLevel->isOnTheWallRight(player2) && (player2->isRightWallJumpAvailable)) {
                    player2->isRightWallJumpAvailable = false;
                    player2->isLeftWallJumpAvailable = true;
                    player2->move(JUMP);
                }
            }
            if (actualKey == Key_Q) {
                if (actualLevel->isInTheDepth(player2)) {
                    player2->move(INTERACTION);
                }
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                player2->move(RIGHT_MOVE);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                player2->move(LEFT_MOVE);
        }
        //exit
        if (actualKey == Key_ESCAPE)
            actualState = GameState_MENU;

        toggleDebug();
    }
    //SINGLE OR MULTIPLAYER
    else if (actualState == GameState_PLAYERS) {
        if (actualKey == Key_UP || actualKey == Key_DOWN)
            multiplayer = !multiplayer;
        if (actualKey == Key_ENTER)
            actualState = GameState_MENU;
        if (actualKey == Key_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    }
    //MENU
    else if (actualState == GameState_MENU) {
        if (actualKey == Key_ENTER) {
            actualState = GameState_GAME;
            if (multiplayer) {
                player = multiplayerLevels[menuChoice]->hero;
                player->resetPosition();
                player2 = multiplayerLevels[menuChoice]->hero2;
                player2->resetPosition();
                actualLevel = multiplayerLevels[menuChoice];
                camera->TeleportToPosition(player->object->graphicsObject->position);
            }
            else {
                player = singleplayerLevels[menuChoice]->hero;
                player->resetPosition();
                actualLevel = singleplayerLevels[menuChoice];
                camera->TeleportToPosition(player->object->graphicsObject->position);
            }
        }
        else if (actualKey == Key_UP) {
            menuChoice--;
            if (menuChoice < 0)
                menuChoice = multiplayer ? multiplayerLevels.size() - 1 : singleplayerLevels.size() - 1;
        }
        else if (actualKey == Key_DOWN) {
            menuChoice++;
            if (multiplayer) {
                if (menuChoice > multiplayerLevels.size() - 1)
                    menuChoice = 0;
            }
            else {
                if (menuChoice > singleplayerLevels.size() - 1)
                    menuChoice = 0;
            }
        }
        else if (actualKey == Key_ESCAPE) {
            actualState = GameState_PLAYERS;
            menuChoice = 0;
        }
    }
    actualKey = Key_NOTHING;
}
void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        actualKey = Key_ENTER;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        actualKey = Key_ESCAPE;
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        actualKey = Key_UP;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        actualKey = Key_DOWN;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        actualKey = Key_SPACE;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        actualKey = Key_W;
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        actualKey = Key_Q;
    }
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

void Game::draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    background->draw();
    if (actualState == GameState_GAME) {
        if (Debug)
            actualLevel->m_pWorld->debugDrawWorld();
        actualLevel->draw(shaders.find("objectShader")->second);
    }
    if (actualState == GameState_PLAYERS) {
        for (std::pair<std::string, UIElement*> element : startMenuUIelements) {
            Shader* shader = shaders.find("fontShader")->second;
            startMenuUIelements.find("menuLogo")->second->draw(shader);
            if (!multiplayer) {
                glm::vec3 oldColor = ((UIText*)startMenuUIelements.find("1 Player")->second)->color;
                ((UIText*)startMenuUIelements.find("1 Player")->second)->color += 0.2f;
                startMenuUIelements.find("1 Player")->second->draw(shader);
                ((UIText*)startMenuUIelements.find("1 Player")->second)->color = oldColor;
                startMenuUIelements.find("2 Players")->second->draw(shader);
            }
            else {
                startMenuUIelements.find("1 Player")->second->draw(shader);
                glm::vec3 oldColor = ((UIText*)startMenuUIelements.find("2 Players")->second)->color;
                ((UIText*)startMenuUIelements.find("2 Players")->second)->color += 0.2f;
                startMenuUIelements.find("2 Players")->second->draw(shader);
                ((UIText*)startMenuUIelements.find("2 Players")->second)->color = oldColor;
            }
        }
    }
    if (actualState == GameState_MENU) {
        Shader* shader = shaders.find("fontShader")->second;
        startMenuUIelements.find("menuLogo")->second->draw(shader);
        int iterator = 0;
        std::map <std::string, UIElement* > levelsMenu = multiplayer ? multiplayerMenuUIelements : singleplayerMenuUIelements;
        for (std::pair<std::string, UIElement*> element : levelsMenu) {
            if (iterator == menuChoice) {
                glm::vec3 oldColor = ((UIText*)element.second)->color;
                ((UIText*)element.second)->color += 0.2f;
                element.second->draw(shader);
                ((UIText*)element.second)->color = oldColor;
            }
            else {
                element.second->draw(shader);
            }
            iterator++;
        }
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
    if (actualState == GameState_GAME) {
        actualLevel->update(deltaTime);

        for (LightObject* light : actualLevel->lights) {
            shaders.find("objectShader")->second->use();
            shaders.find("objectShader")->second->setVec3("lightPosition", light->object->graphicsObject->position);
        }

    if (!Debug) {
        if(multiplayer)
            camera->WatchObject2(player->object->graphicsObject->position, player2->object->graphicsObject->position, 0.9, deltaTime);
        else
            camera->WatchObject(player->object->graphicsObject->position, 0.9, deltaTime);
    }
        //ENDCASE
        if (!multiplayer) {
            if (glm::distance(actualLevel->diamond->object->graphicsObject->position, player->object->graphicsObject->position) < 1) {
                if (menuChoice == (singleplayerLevels.size() - 1)) {
                    actualState = GameState_MENU;
                }
                else {
                    menuChoice++;
                    player = singleplayerLevels[menuChoice]->hero;
                    player->resetPosition();
                    actualLevel = singleplayerLevels[menuChoice];
                }
            }
        }
        else if (multiplayer) {
            if (glm::distance(actualLevel->diamond->object->graphicsObject->position, player->object->graphicsObject->position) < 1 || (glm::distance(actualLevel->diamond->object->graphicsObject->position, player2->object->graphicsObject->position) < 1)) {
                if (menuChoice == (multiplayerLevels.size() - 1)) {
                    actualState = GameState_MENU;
                }
                else {
                    menuChoice++;
                    player = multiplayerLevels[menuChoice]->hero;
                    player->resetPosition();
                    player2 = multiplayerLevels[menuChoice]->hero2;
                    player2->resetPosition();
                    actualLevel = multiplayerLevels[menuChoice];
                }
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
