#include <cassert>

#include "Game.hpp"
#include "GLUtils.hpp"
#include <stdio.h>

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

        while (!glfwWindowShouldClose(window))
        {
            MainLoop();
        }

        glfwTerminate();
    }

    void Game::InitializeGLFW()
    {
        assert(glfwInit() && "GLFW was unable to initialize.");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        window = glfwCreateWindow(640, 480, "Window", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            assert(false && "Failed to create glfw window.");
        }

        glfwMakeContextCurrent(window);
    }

    void Game::InitializeGLAD()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            glfwTerminate();
            assert(false && "GLAD was unable to load openGL.");
        }

        GLUtils::checkForOpenGLError(__FILE__, __LINE__);
        GLUtils::dumpGLInfo();
    }

    void Game::InitializeGL()
    {
        glad_glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
        glad_glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void Game::InitializeCallbacks()
    {
        glfwSetErrorCallback(Game::GLFWErrorCallback);
    }

    void Game::GLFWErrorCallback(int error, const char* description)
    {
        fputs(description, stderr);
    }

    void Game::MainLoop()
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }
}