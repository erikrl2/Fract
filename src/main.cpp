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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(640, 480, APP_NAME, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowSizeLimits(window, 200, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);

    static float zoom = 1.0f;
    static bool fractToggle = false;

    glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) {
        if (yoffset > 0)
            zoom /= 1.2f;
        else if (yoffset < 0)
            zoom *= 1.2f;
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
            fractToggle = !fractToggle;
    });
    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            fractToggle = !fractToggle;
    });

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(0); // Vsync off

    GLuint program, va, vb, ib;

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
    GLint mandelbLocation = glGetUniformLocation(program, "mandelb");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        updateWindowTitle(window);

        static double lastT = 0.0;
        double t = glfwGetTime();
        float dt = float(t - lastT);
        lastT = t;

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        static vec2 startOffset = {0.0f, 0.0f};

        float baseRes = 2.0f / std::min(width, height);
        float res = baseRes * zoom;

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            zoom /= 1.0f + dt;
        else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            zoom *= 1.0f + dt;

        float offset = 500.0f * res * dt;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            startOffset[1] += offset;
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            startOffset[1] -= offset;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            startOffset[0] -= offset;
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            startOffset[0] += offset;

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
                vec2_sub(startOffset, startOffset, delta);
            } else
                pressed = true;

            vec2_dup(lastMousePos, mousePos);
        } else
            pressed = false;

        vec2 start = {-width * 0.5f * res, -height * 0.5f * res};
        vec2_add(start, start, startOffset);

        float f = std::log10(1 / zoom) / 6.0f;
        uint32_t n = 50 + uint32_t(std::clamp(f, 0.0f, 1.0f) * 1000);

        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glUniform2fv(startLocation, 1, start);
        glUniform1f(resLocation, res);
        glUniform1ui(nLocation, n);
        glUniform1i(mandelbLocation, fractToggle);

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
