#include "Window.h"

#include "Fract.h"

#include <glad/glad.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

namespace Fract {

    Window::Window(const std::string& title, IVec2 size, bool vSync)
        : handle(CreateWindow(title.c_str(), size.x, size.y, vSync)), size(size), vSync(vSync)
    {
        SetWindowCallbacks();
        SetupImGui();
    }

    Window::~Window()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(handle);
        glfwTerminate();
    }

    GLFWwindow* Window::CreateWindow(const char* title, int width, int height, bool vsync)
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

    void Window::SetWindowCallbacks()
    {
        glfwSetScrollCallback(handle, [](GLFWwindow* w, double xoffset, double yoffset) {
            FractApp* app = (FractApp*)glfwGetWindowUserPointer(w);
            app->OnMouseScroll(yoffset);
            });
        glfwSetMouseButtonCallback(handle, [](GLFWwindow* w, int button, int action, int mods) {
            FractApp* app = (FractApp*)glfwGetWindowUserPointer(w);
            if (action == GLFW_PRESS)
                app->OnMouseButtonPress(button);
            });
        glfwSetKeyCallback(handle, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
            FractApp* app = (FractApp*)glfwGetWindowUserPointer(w);
            if (action == GLFW_PRESS)
                app->OnKeyPress(key);
            });
        glfwSetFramebufferSizeCallback(handle, [](GLFWwindow* w, int width, int height) {
            glViewport(0, 0, width, height);
            });
        glfwSetWindowSizeCallback(handle, [](GLFWwindow* w, int width, int height) {
            FractApp* app = (FractApp*)glfwGetWindowUserPointer(w);
            app->SetWindowSize({ width, height });
            });
    }

    void Window::SetupImGui()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = nullptr;

        ImGui_ImplGlfw_InitForOpenGL(handle, true);
        ImGui_ImplOpenGL3_Init();
    }

    float Window::GetDeltaTime() const
    {
        double t = glfwGetTime();
        static double lastT = t;
        float dt = float(t - lastT);
        lastT = t;
        return dt;
    }

}