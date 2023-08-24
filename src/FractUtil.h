#pragma once

#include <GLFW/glfw3.h>

#include <cstdint>

namespace Fract {

    template<typename T>
    struct Vec2
    {
        T x, y;

        Vec2<T> operator+(Vec2<T> other) { return { x + other.x, y + other.y }; }
        Vec2<T> operator-(Vec2<T> other) { return { x - other.x, y - other.y }; }
        Vec2<T> operator*(float scalar) { return { x * scalar, y * scalar }; }

        Vec2<T>& operator+=(Vec2<T> other) { x += other.x; y += other.y; return *this; }
        Vec2<T>& operator-=(Vec2<T> other) { x -= other.x; y -= other.y; return *this; }

        T* operator&() { return &x; }

        template<typename U>
        operator Vec2<U>() { return { (U)x, (U)y }; }
    };

    using vec2 = Vec2<float>;
    using ivec2 = Vec2<int>;

    struct Window
    {
        GLFWwindow* Handle;
        ivec2 Size;

        GLFWwindow* operator&() const { return Handle; }
    };

    struct FractData
    {
        vec2 start;
        uint32_t n;
        float res, zoom = 1;
        bool fractToggle = false;
    };

    inline void toggleFullscreen(const Window& window)
    {
        static bool fullscreen = true;
        static ivec2 pos, size;

        if (fullscreen)
        {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwGetWindowPos(&window, &pos.x, &pos.y);
            glfwGetWindowSize(&window, &size.x, &size.y);
            glfwSetWindowMonitor(&window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
        }
        else
        {
            glfwSetWindowMonitor(&window, NULL, pos.x, pos.y, size.x, size.y, GLFW_DONT_CARE);
        }

        fullscreen = !fullscreen;
    }

    inline vec2 getMousePosDelta(const Window& window)
    {
        double xpos, ypos;
        glfwGetCursorPos(&window, &xpos, &ypos);

        vec2 mousePos{ float(xpos), window.Size.y - float(ypos) };
        static vec2 lastMousePos = mousePos;

        vec2 mouseDelta = mousePos - lastMousePos;

        lastMousePos = mousePos;

        return mouseDelta;
    }

}
