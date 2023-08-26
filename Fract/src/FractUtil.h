#pragma once

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <cstdint>

namespace Fract {

    template<typename T>
    struct Vec2
    {
        T X, Y;

        Vec2() : X(0), Y(0) {}
        Vec2(T X, T Y) : X(X), Y(Y) {}
        Vec2(ImVec2 vec2) : X(vec2.x), Y(vec2.y) {}

        Vec2<T> operator+(Vec2<T> other) { return { X + other.X, Y + other.Y }; }
        Vec2<T> operator-(Vec2<T> other) { return { X - other.X, Y - other.Y }; }
        Vec2<T> operator*(float scalar) { return { X * scalar, Y * scalar }; }

        Vec2<T>& operator+=(Vec2<T> other) { X += other.X; Y += other.Y; return *this; }
        Vec2<T>& operator-=(Vec2<T> other) { X -= other.X; Y -= other.Y; return *this; }

        T* operator&() { return &X; }

        template<typename U>
        operator Vec2<U>() { return { (U)X, (U)Y }; }
    };

    using vec2 = Vec2<float>;
    using ivec2 = Vec2<int>;

    struct Window
    {
        GLFWwindow* Handle;
        ivec2 Size;

        GLFWwindow* operator&() const { return Handle; }
    };

    inline void setFullscreen(const Window& window, bool fullscreen = true)
    {
        static ivec2 pos, size;

        if (fullscreen)
        {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwGetWindowPos(&window, &pos.X, &pos.Y);
            glfwGetWindowSize(&window, &size.X, &size.Y);
            glfwSetWindowMonitor(&window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
        }
        else
        {
            glfwSetWindowMonitor(&window, NULL, pos.X, pos.Y, size.X, size.Y, GLFW_DONT_CARE);
        }
    }

    inline vec2 getMousePosDelta(const Window& window)
    {
        double xpos, ypos;
        glfwGetCursorPos(&window, &xpos, &ypos);

        vec2 mousePos{ float(xpos), window.Size.Y - float(ypos) };
        static vec2 lastMousePos = mousePos;

        vec2 mouseDelta = mousePos - lastMousePos;

        lastMousePos = mousePos;

        return mouseDelta;
    }

}
