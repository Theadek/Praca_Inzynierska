#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "TextureManager.h"
#include "Shader.h"
#include "Object.h"
#include "Game.h"
#include <string>
#include "background.h"


glm::vec3 lightPos(4.0f, 4.0f, 0.0f);

Game game = Game(800, 600);


int main()
{
    game.init();
    Background background = Background();

    //Object* light = new Object();
    //light->position = lightPos;
    //light->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    //light->rotate = 0.0f;
    //light->model = &game.models.find("cube")->second;


    GraphicsObject* graphicsObject = new GraphicsObject(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, &game.models.find("cube")->second);
    PhysicsObject* physicsObject = new PhysicsObject(graphicsObject, 1.0f);
    Object* cube = new Object(graphicsObject, physicsObject);
    cube->physicsObject->pRigidBody->setLinearFactor(btVector3(1.0f, 1.0f, 1.0f));
    cube->physicsObject->pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
    cube->physicsObject->pRigidBody->setFriction(1.0f);
    cube->physicsObject->pRigidBody->setRestitution(0.0f);

    GraphicsObject* graphicsObject2 = new GraphicsObject(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(10.0f, 0.5f, 1.0f), 0.0f, &game.models.find("cube")->second);
    PhysicsObject* physicsObject2 = new PhysicsObject(graphicsObject2, 0.0f);
    Object* floor2 = new Object(graphicsObject2, physicsObject2);
    floor2->physicsObject->pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
    floor2->physicsObject->pRigidBody->setFriction(1.0f);
    floor2->physicsObject->pRigidBody->setRestitution(0.0f);

    GraphicsObject* graphicsObject3 = new GraphicsObject(glm::vec3(2.0f, 2.0f, -5.0f), glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, &game.models.find("cube")->second);
    PhysicsObject* physicsObject3 = new PhysicsObject(graphicsObject3, 0.0f);
    Object* floor3 = new Object(graphicsObject3, physicsObject3);
    floor3->physicsObject->pRigidBody->setLinearFactor(btVector3(1.0f, 1.0f, 1.0f));
    floor3->physicsObject->pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
    floor3->physicsObject->pRigidBody->setFriction(1.0f);
    floor3->physicsObject->pRigidBody->setRestitution(0.0f);
    floor3->physicsObject->pRigidBody->setCollisionFlags(floor3->physicsObject->pRigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    Hero* hero = new Hero(cube);
    game.player = hero;
    game.objects.push_back(cube);
    game.objects.push_back(floor2);
    game.objects.push_back(floor3);
    game.m_pWorld->addRigidBody(cube->physicsObject->pRigidBody);
    game.m_pWorld->addRigidBody(floor2->physicsObject->pRigidBody);
    game.m_pWorld->addRigidBody(floor3->physicsObject->pRigidBody);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(game.window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game.update();
        //these lines should go to game::update metod
        glm::mat4 projection = glm::perspective(glm::radians(game.camera->Zoom), (float)game.SCR_WIDTH / (float)game.SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = game.camera->GetViewMatrix();
        //glm::mat4 view = glm::lookAt(glm::vec3(game.SCR_WIDTH / 2 * 0.01, game.SCR_HEIGHT / 2 * 0.01, 10.0f), glm::vec3(game.SCR_WIDTH / 2 * 0.01, game.SCR_HEIGHT / 2 * 0.01, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, light->position);


        //each object shoudl have pointer to shader or flag which determines shader
        game.shaders.find("objectShader")->second->use();
        game.shaders.find("objectShader")->second->setMat4("view", view);
        game.shaders.find("objectShader")->second->setMat4("projection", projection);
        game.shaders.find("objectShader")->second->setVec3("viewPos", game.camera->Position);
        game.shaders.find("objectShader")->second->setVec3("lightPosition", lightPos);
        //floor->renderModel(game.shaders.find("objectShader")->second);

        /*game.shaders.find("lightShader")->second->use();
        game.shaders.find("lightShader")->second->setMat4("view", view);
        game.shaders.find("lightShader")->second->setMat4("projection", projection);
        game.shaders.find("lightShader")->second->setMat4("model", model);
        light->renderModel(game.shaders.find("lightShader")->second);*/

        background.draw();
        game.draw();
        glfwSwapBuffers(game.window);
        glfwPollEvents();
    }

    return 0;
}
