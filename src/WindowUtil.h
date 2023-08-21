#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

inline GLFWwindow* createWindow(const char* title, int width, int height, bool vsync = true)
{
    glfwSetErrorCallback([](int error, const char* description) {
        fprintf(stderr, "Error: %s\n", description);
    });

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowSizeLimits(window, 200, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(vsync);

    return window;
}

inline void shutdown(GLFWwindow* window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
