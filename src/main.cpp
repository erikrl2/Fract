#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "shaders.h"
#include "util.hpp"

#define APP_NAME "Fract"

static GLuint createShader(GLenum type, const char* shaderSrc)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    GLint status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        std::vector<GLchar> infoLog(length);
        glGetShaderInfoLog(shader, length, &length, &infoLog[0]);
        glDeleteShader(shader);

        fprintf(stderr, "%s\n", infoLog.data());
    }

    return shader;
}

static GLuint createProgram()
{
    std::vector<GLuint> shaders;
    shaders.push_back(createShader(GL_VERTEX_SHADER, vertexShaderSrc.c_str()));
    shaders.push_back(createShader(GL_FRAGMENT_SHADER, fragmentShaderSrc.c_str()));

    GLuint program = glCreateProgram();

    for (GLuint shader : shaders)
        glAttachShader(program, shader);

    glLinkProgram(program);

    for (GLuint shader : shaders)
        glDetachShader(program, shader);

    std::for_each(shaders.begin(), shaders.end(), glDeleteShader);

    return program;
}

static void updateWindowTitle(GLFWwindow *window)
{
    static double lastTime = 0.0;
    static size_t nbFrames = 0;

    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    nbFrames++;
    if (delta >= 1.0)
    {
        double fps = double(nbFrames) / delta;

        std::stringstream ss;
        ss << APP_NAME << " [" << fps << " FPS]";

        glfwSetWindowTitle(window, ss.str().c_str());

        nbFrames = 0;
        lastTime = currentTime;
    }
}

int main(int argc, char* argv[])
{
    glfwSetErrorCallback([](int error, const char* description) {
        fprintf(stderr, "Error: %s\n", description);
    });

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, APP_NAME, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowSizeLimits(window, 200, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);

    static double zoom = 1.0;
    static bool fractToggle = false;

    glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) {
        if (yoffset > 0)
            zoom /= 1.2;
        else if (yoffset < 0)
            zoom *= 1.2;
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
            fractToggle = !fractToggle;
    });
    static uint32_t n = 50;
    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            fractToggle = !fractToggle;
    });

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(0); // Vsync off

    GLuint program, vao, vbo, ibo;

    program = createProgram();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    float vertices[4][2] =
    {
        {  1.0f, -1.0f },
        { -1.0f, -1.0f },
        { -1.0f,  1.0f },
        {  1.0f,  1.0f }
    };
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*) 0);

    uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLint startLocation = glGetUniformLocation(program, "start");
    GLint resLocation = glGetUniformLocation(program, "res");
    GLint nLocation = glGetUniformLocation(program, "n");
    GLint mandelbLocation = glGetUniformLocation(program, "mandelb");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        updateWindowTitle(window);

        static double lastT = 0.0;
        double t = glfwGetTime();
        double dt = t - lastT;
        lastT = t;

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        using namespace Fract;

        static Vec2<double> startOffset = {0.0, 0.0};

        double res = 2.0 / std::min(width, height) * zoom;

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            zoom /= 1.0 + dt;
        else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            zoom *= 1.0 + dt;

        double offset = 500 * res * dt;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            startOffset.y += offset;
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            startOffset.y -= offset;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            startOffset.x -= offset;
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            startOffset.x += offset;

        static bool pressed = false;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            Vec2<double> mousePos;
            glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
            mousePos.y = height - mousePos.y;

            static Vec2<double> lastMousePos;

            if (pressed)
                startOffset -= (mousePos - lastMousePos) * res;
            else
                pressed = true;

            lastMousePos = mousePos;
        } else
            pressed = false;

        Vec2<double> start{-width * 0.5f * res, -height * 0.5f * res};
        start += startOffset;

        double f = std::log10(1 / zoom) / 14.0;
        uint32_t n = 50 + uint32_t(std::clamp(f, 0.0, 1.0) * 200);

        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glUniform2dv(startLocation, 1, &start);
        glUniform1d(resLocation, res);
        glUniform1ui(nLocation, n);
        glUniform1i(mandelbLocation, fractToggle);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
