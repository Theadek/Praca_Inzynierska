#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ctime>
#include <iostream>
#include "Game.h"


Game game = Game(1600, 1200);
double clockToMilliseconds(clock_t ticks) {
    return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
}
clock_t deltaTime = 0;
unsigned int frames = 0;
double  frameRate = 30;
double  averageFrameTimeMilliseconds = 33.333;
int main()
{
    game.init();

    while (!glfwWindowShouldClose(game.window))
    {
        game.update();
        clock_t beginFrame = clock();
        game.draw();
        clock_t endFrame = clock();
        deltaTime += endFrame - beginFrame;
        frames++;

        //if you really want FPS
        if (clockToMilliseconds(deltaTime) > 1000.0) { //every second
            frameRate = (double)frames * 0.5 + frameRate * 0.5; //more stable
            frames = 0;
            deltaTime -= CLOCKS_PER_SEC;
            averageFrameTimeMilliseconds = 1000.0 / (frameRate == 0 ? 0.001 : frameRate);
            std::cout << "Time needed to draw:" << averageFrameTimeMilliseconds << std::endl;
        }
    }

    return 0;
}
