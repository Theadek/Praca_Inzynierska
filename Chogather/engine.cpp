#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Game.h"


Game game = Game(1600, 1200);


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
