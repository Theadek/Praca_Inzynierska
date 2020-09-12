#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Object.h"
#include "Game.h"
#include <string>

glm::vec3 lightPos(0.2f, 4.0f, 0.0f);
Game game = Game(800, 600);

int main()
{
    game.init();

    float background[] = {
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f,    //right top
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f,   // right bottom
        -1.0f, -1.0f,-1.0f,  0.0f, 0.0f,  //left bottom
        -1.0f, 1.0f,-1.0f,  0.0f, 1.0f    //left top
    };
    unsigned int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(background), background, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    ShaderManager backgroundShader("Shaders/background.vs", "Shaders/background.fs");
    backgroundShader.use();
    Texture2D backgroundTex = Texture2D();
    backgroundTex.Generate("Textures/background.jpg");
    backgroundShader.setInt("texture1", 3);
    glActiveTexture(GL_TEXTURE3);
    backgroundTex.Bind();

    ShaderManager lightShader("Shaders/light.vs", "Shaders/light.fs");

    ShaderManager ourShader("Shaders/vertexShader.vs", "Shaders/fragmentShader.fs");

    Object* light = new Object();
    light->position = lightPos;
    light->size = glm::vec3(1.0f, 1.0f, 1.0f);
    light->rotate = 0.0f;

    Object* backpack = new Object();
    backpack->position = glm::vec3(1.0f, 0.0f, -2.0f);
    backpack->size = glm::vec3(1.0f, 1.0f, 1.0f);
    backpack->rotate = 0.0f;

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
        glm::mat4 projection = glm::perspective(glm::radians(game.camera->Zoom), (float)game.SCR_WIDTH / (float)game.SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = game.camera->GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        backgroundShader.use();
        backgroundTex.Bind();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glEnable(GL_DEPTH_TEST);

        //light source render
        light->position.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        light->position.y = sin(glfwGetTime() / 2.0f) * 1.0f;
        model = glm::mat4(1.0f);
        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        lightShader.setMat4("model", model);
        //for testing
        //game.models.find("cube")->second->renderModel(&lightShader, light);

        //backpack render
        ourShader.use();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setVec3("viewPos", game.camera->Position);
        ourShader.setVec3("lightPosition", light->position);
        //for testing
        //game.models.find("cube")->second->renderModel(&ourShader, backpack);

        glfwSwapBuffers(game.window);
        glfwPollEvents();
    }

    return 0;
}
