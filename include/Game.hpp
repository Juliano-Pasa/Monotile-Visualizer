#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "glad.h"
#include "glm/glm.hpp"

#include <GLFW/glfw3.h>

#include "Renderer.hpp"

namespace monotile
{
    using namespace std;
    using namespace monotile;

    class Game
    {
    public:
        Game();
        void Start();

    private:
        void InitializeGLFW();
        void InitializeGLAD();
        void InitializeGL();
        void InitializeCallbacks();

        static void GLFWErrorCallback(int error, const char* description);

        void MainLoop();

        GLFWwindow* window;
        Renderer* renderer;
    };
}

#endif // __GAME_HPP__