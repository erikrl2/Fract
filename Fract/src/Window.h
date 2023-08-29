#pragma once

#include "Util.h"

#include <GLFW/glfw3.h>

#include <string>

namespace Fract {

    class Window
    {
    public:
        Window(const std::string& title, IVec2 size, bool vSync);
        ~Window();

        GLFWwindow* operator&() const { return handle; }
    private:
        void Window::CreateWindow(const char* title);

        void SetWindowCallbacks();

        void SetupImGui();
    public:
        IVec2 GetFrambufferSize() const;
        float GetDeltaTime() const;
    public:
        GLFWwindow* handle;
        const GLFWvidmode* mode;

        IVec2 size;
        bool vSync;
    };


}
