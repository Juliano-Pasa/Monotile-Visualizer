#include <cassert>

#include "Game.hpp"
#include "GLUtils.hpp"

namespace Monotile
{
    Game::Game()
    {

    }

    void Game::Start()
    {
        InitializeGLFW();
        InitializeGLAD();
        InitializeGL();

        GLUtils::dumpGLInfo();

        while (!glfwWindowShouldClose(window))
        {
            MainLoop();
        }

        glfwTerminate();
    }

    void Game::InitializeGLFW()
    {
        assert(glfwInit() && "GLFW was unable to initialize.");

        window = glfwCreateWindow(640, 480, "Window", NULL, NULL);
        glfwMakeContextCurrent(window);
    }

    void Game::InitializeGLAD()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            glfwTerminate();
            assert(false && "GLAD was unable to load openGL.");
        }
    }

    void Game::InitializeGL()
    {
        glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    }

    void Game::MainLoop()
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }
}