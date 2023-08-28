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
        GLFWwindow* CreateWindow(const char* title, int width, int height, bool vsync = true);

        void SetWindowCallbacks();

        void SetupImGui();
    public:
        float GetDeltaTime() const;
    private:
        GLFWwindow* handle;
    public:
        IVec2 size;
        bool vSync;
    };


}
