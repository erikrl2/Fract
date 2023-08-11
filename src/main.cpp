#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <linmath.h>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>

#include "shaders.h"

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

static void showFPS(GLFWwindow *window)
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

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static float zoom = 1.0f;

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset > 0)
        zoom /= 1.2f;
    else if (yoffset < 0)
        zoom *= 1.2f;
}

int main(int argc, char* argv[])
{
    GLFWwindow* window;
    GLuint va, vb, ib, program;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(640, 480, APP_NAME, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowSizeLimits(window, 200, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwSetScrollCallback(window, scroll_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(0);

    program = createProgram();

    glGenVertexArrays(1, &va);
    glBindVertexArray(va);

    float vertices[4][2] =
    {
        {  1.0f, -1.0f },
        { -1.0f, -1.0f },
        { -1.0f,  1.0f },
        {  1.0f,  1.0f }
    };
    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*) 0);

    uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLint startLocation = glGetUniformLocation(program, "start");
    GLint resLocation = glGetUniformLocation(program, "res");
    GLint nLocation = glGetUniformLocation(program, "n");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        showFPS(window);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        float baseRes = 2.0f / std::min(width, height);
        float res = baseRes * zoom;

        vec2 start = {-width * 0.5f * res, -height * 0.5f * res};
        static vec2 offset = {0.0f, 0.0f};
        static bool pressed = false;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            vec2 mousePos = {float(xpos), height - float(ypos)};
            static vec2 lastMousePos;

            if (pressed)
            {
                vec2 delta{};
                vec2_sub(delta, mousePos, lastMousePos);
                vec2_scale(delta, delta, res);
                vec2_sub(offset, offset, delta);
            } else
                pressed = true;

            vec2_dup(lastMousePos, mousePos);
        } else
            pressed = false;

        vec2_add(start, start, offset);

        float f = std::log10(1 / zoom) / 6.0f;
        uint32_t n = 50 + uint32_t(std::clamp(f, 0.0f, 1.0f) * 1000);

        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glUniform2fv(startLocation, 1, start);
        glUniform1f(resLocation, res);
        glUniform1ui(nLocation, n);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &vb);
    glDeleteBuffers(1, &ib);
    glDeleteVertexArrays(1, &va);
    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
