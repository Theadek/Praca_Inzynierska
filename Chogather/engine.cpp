#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Object.h"
#include "Game.h"
#include <string>
#include "background.h"
glm::vec3 lightPos(0.2f, 4.0f, 0.0f);
Game game = Game(800, 600);
int main()
{
    game.init();

    Background background = Background();

    Object* light = new Object();
    light->position = lightPos;
    light->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    light->rotate = 0.0f;
    light->model = game.models.find("cube")->second;

    Object* floor = new Object();
    floor->position = glm::vec3(2.0f, 0.0f, 0.0f);
    floor->rotate = 0.0f;
    floor->scale = glm::vec3(2.0f, 3.0f, 1.0f);
    floor->model = game.models.find("cube")->second;
    game.player->hero->model = game.models.find("cube")->second;


    game.objects.push_back(floor);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(game.window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        game.deltaTime = currentFrame - game.lastFrame;
        game.lastFrame = currentFrame;

        // input
        // -----
        game.processInput(game.window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //background render
        glDisable(GL_DEPTH_TEST);
        background.draw();


        glm::mat4 projection = glm::perspective(glm::radians(game.camera->Zoom), (float)game.SCR_WIDTH / (float)game.SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 view = game.camera->GetViewMatrix();


        //szczerze to jest kurwa zart jakis
        //trzeba zrobic tak, zeby pkt 0,0 byl w lewym dolnym rogu
        // jak? nie wiem, jest prawie ok xD
        glm::mat4 view = glm::lookAt(glm::vec3(game.SCR_WIDTH / 2 * 0.01, game.SCR_HEIGHT / 2 * 0.01, 10.0f), glm::vec3(game.SCR_WIDTH / 2 * 0.01, game.SCR_HEIGHT / 2 * 0.01, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 model = glm::mat4(1.0f);




        game.shaders.find("basicShader")->second->use();
        game.shaders.find("basicShader")->second->setMat4("view", view);
        game.shaders.find("basicShader")->second->setMat4("projection", projection);
        game.shaders.find("basicShader")->second->setVec3("viewPos", game.camera->Position);
        game.shaders.find("basicShader")->second->setVec3("lightPosition", light->position);
        //for testing
        if (game.detectCollision(game.player->hero, floor))
        {
            std::cout << "Collision\n";
        }
        else {
            std::cout << "No\n";
        }
        game.player->hero->renderModel(game.shaders.find("basicShader")->second);
        floor->renderModel(game.shaders.find("basicShader")->second);
        glfwSwapBuffers(game.window);
        glfwPollEvents();
    }

    return 0;
}
