#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Game.h"


Game game = Game(800, 600);


int main()
{
    game.init();

    while (!glfwWindowShouldClose(game.window))
    {
        game.update();
        game.draw();
    }

    return 0;
}
