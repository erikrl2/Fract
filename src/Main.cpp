#include "App.h"
#include "Util.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <unordered_map>

GLFWwindow* Fract::window;

static void setWindowCallbacks()
{
    using namespace Fract;
    glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) { OnMouseScroll(yoffset); });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) { if (action == GLFW_PRESS) OnMouseButtonPress(button); });
    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) { if (action == GLFW_PRESS) OnKeyPress(key); });
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* w, int width, int height) { glViewport(0, 0, width, height); });
}

static GLuint program, vao, vbo, ibo;

static void setupDraw()
{
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

static void shutdown()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);

    glfwDestroyWindow(Fract::window);
    glfwTerminate();
}

int main(int argc, char* argv[])
{
	using namespace Fract;

    window = createWindow("Fract", 640, 480);
    setWindowCallbacks();

    program = createProgram();
    setupDraw();

    std::unordered_map<std::string, GLint> uniformLocations;
    uniformLocations["start"] = glGetUniformLocation(program, "start");
    uniformLocations["res"] = glGetUniformLocation(program, "res");
    uniformLocations["n"] = glGetUniformLocation(program, "n");
    uniformLocations["mandelb"] = glGetUniformLocation(program, "mandelb");

	glUseProgram(program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        static double lastT = 0.0;
        double t = glfwGetTime();
        float ts = float(t - lastT);
        lastT = t;

        Update(ts);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Draw(uniformLocations);

        glfwSwapBuffers(window);
    }

    return EXIT_SUCCESS;
}
