#include "Window.h"

#include "Fract.h"

#include <glad/glad.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

namespace Fract {

    Window::Window(const std::string& title, IVec2 size, bool vSync)
        : size(size), vSync(vSync)
    {
        CreateWindow(title.c_str());
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

    void Window::CreateWindow(const char* title)
    {
        glfwSetErrorCallback([](int error, const char* description) {
            std::cerr << "Error: " << description << std::endl;
        });

        if (!glfwInit())
            exit(EXIT_FAILURE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

        handle = glfwCreateWindow(size.x, size.y, title, NULL, NULL);
        if (!handle)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowSizeLimits(handle, 325, 325, mode->width, mode->height);

        size = GetFrambufferSize();

        glfwMakeContextCurrent(handle);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwSwapInterval(vSync);
    }

    void Window::SetWindowCallbacks()
    {
        glfwSetScrollCallback(handle, [](GLFWwindow* w, double xoffset, double yoffset) {
            ((FractApp*)glfwGetWindowUserPointer(w))->OnMouseScroll(yoffset);
        });
        glfwSetMouseButtonCallback(handle, [](GLFWwindow* w, int button, int action, int mods) {
            if (action == GLFW_PRESS)
                ((FractApp*)glfwGetWindowUserPointer(w))->OnMouseButtonPress(button);
        });
        glfwSetKeyCallback(handle, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
            if (action == GLFW_PRESS)
                ((FractApp*)glfwGetWindowUserPointer(w))->OnKeyPress(key);
        });
        glfwSetFramebufferSizeCallback(handle, [](GLFWwindow* w, int width, int height) {
            ((FractApp*)glfwGetWindowUserPointer(w))->SetWindowSize({ width, height });
            glViewport(0, 0, width, height);
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

    IVec2 Window::GetFrambufferSize() const
    {
        int width, height;
        glfwGetFramebufferSize(handle, &width, &height);
        return IVec2(width, height);
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