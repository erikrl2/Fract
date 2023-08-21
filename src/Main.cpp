#include "App.h"
#include "WindowUtil.h"
#include "RenderUtil.h"

#include <unordered_map>

Fract::Window Fract::window{};

static RenderData rData{};

static void setWindowCallbacks();

int main(int argc, char* argv[])
{
	using namespace Fract;

    ivec2 windowSize{ 640, 480 };

    window.handle = createWindow("Fract", windowSize.x, windowSize.y);
    window.viewportSize = windowSize;

    setWindowCallbacks();

    rData.program = createProgram();

    setupDraw(rData);

    std::unordered_map<std::string, GLint> uniformLocations;
    uniformLocations["start"] = glGetUniformLocation(rData.program, "start");
    uniformLocations["res"] = glGetUniformLocation(rData.program, "res");
    uniformLocations["n"] = glGetUniformLocation(rData.program, "n");
    uniformLocations["mandelb"] = glGetUniformLocation(rData.program, "mandelb");

	glUseProgram(rData.program);
	glBindVertexArray(rData.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rData.ibo);

    while (!glfwWindowShouldClose(&window))
    {
        glfwPollEvents();

        static double lastT = 0.0;
        double t = glfwGetTime();
        float ts = float(t - lastT);
        lastT = t;

        Update(ts);

        Draw(uniformLocations);

        glfwSwapBuffers(&window);
    }

    cleanup(rData);
    shutdown(&window);

    return EXIT_SUCCESS;
}

static void setWindowCallbacks()
{
    using namespace Fract;
    glfwSetScrollCallback(&window, [](GLFWwindow* w, double xoffset, double yoffset) {
        OnMouseScroll(yoffset);
	});
    glfwSetMouseButtonCallback(&window, [](GLFWwindow* w, int button, int action, int mods) {
        if (action == GLFW_PRESS)
            OnMouseButtonPress(button);
	});
    glfwSetKeyCallback(&window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS)
            OnKeyPress(key);
	});
    glfwSetFramebufferSizeCallback(&window, [](GLFWwindow* w, int width, int height) {
        glViewport(0, 0, width, height);
        window.viewportSize = { width, height };
	});
}
