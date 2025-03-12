#include <cassert>

#include "Game.hpp"
#include "GLUtils.hpp"
#include <stdio.h>

namespace monotile
{
    Game::Game()
    {
    }

    void Game::Start()
    {
        InitializeGLFW();
        InitializeGLAD();
        InitializeGL();

        renderer = new Renderer(window);
        renderer->Initialize(3);

        MainLoop();
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
        glad_glClearColor(0.76f, 0.39f, 0.36f, 1.0f);
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
        double thisTime;
        double lastTime = glfwGetTime();

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            GLUtils::checkForOpenGLError(__FILE__, __LINE__);

            thisTime = glfwGetTime();
            renderer->Update(thisTime - lastTime);
            lastTime = thisTime;

            renderer->Render();
            glfwSwapBuffers(window);
        }
    }
}