#include "glad.h"
#include "glm/glm.hpp"

#include <iostream>
#include <GLFW/glfw3.h>

#include "GLUtils.hpp"

int main(int, char**){
    GLFWwindow* window;

    if (!glfwInit())
    {
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Window", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Couldn't load opengl\n";
        glfwTerminate();
        return -1;
    }

    GLUtils::dumpGLInfo();

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    glm::vec2 vector = glm::vec2(0, 1);

    std::cout << vector.x << " " << vector.y << "\n";

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
