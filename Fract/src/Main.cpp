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

    window.VSync = true;
    window.Size = { 1280, 720 };
    window.Handle = createWindow("Fract", window.Size.X, window.Size.Y, window.VSync);

    setWindowCallbacks();

    setupImGui(&window);

    rData.program = createProgram();
    setupDraw(rData);

    std::unordered_map<std::string, GLint> uniformLocations;
    uniformLocations["start"] = glGetUniformLocation(rData.program, "start");
    uniformLocations["res"] = glGetUniformLocation(rData.program, "res");
    uniformLocations["n"] = glGetUniformLocation(rData.program, "n");
    uniformLocations["theme"] = glGetUniformLocation(rData.program, "theme");
    uniformLocations["color"] = glGetUniformLocation(rData.program, "color");
    uniformLocations["isMandelbrot"] = glGetUniformLocation(rData.program, "isMandelbrot");

    glUseProgram(rData.program);
    glBindVertexArray(rData.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rData.ibo);

    Init();

    while (!glfwWindowShouldClose(&window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float dt = getDeltaTime();
        Update(dt);
        UpdateImGui(dt);

        Draw(uniformLocations);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
    });
    glfwSetWindowSizeCallback(&window, [](GLFWwindow* w, int width, int height) {
        window.Size = { width, height };
    });
}
