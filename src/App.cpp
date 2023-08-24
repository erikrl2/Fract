#include "App.h"

#include <algorithm>
#include <cmath>

namespace Fract {

    static FractData fData{};

    static void updateStart(float ts);
    static void updateRes(float ts);
    static void updateN();

    void Update(float ts)
    {
        updateRes(ts);
        updateStart(ts);
        updateN();
    }

    void Draw(const std::unordered_map<std::string, GLint>& uniformLocations)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform2fv(uniformLocations.at("start"), 1, &fData.start);
        glUniform1f(uniformLocations.at("res"), fData.res);
        glUniform1ui(uniformLocations.at("n"), fData.n);
        glUniform1i(uniformLocations.at("mandelb"), fData.fractToggle);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void OnMouseScroll(double offset)
    {
        if (offset > 0.0)
            fData.zoom /= 1.2f;
        else if (offset < 0.0)
            fData.zoom *= 1.2f;
    }

    void OnMouseButtonPress(int button)
    {
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
            fData.fractToggle = !fData.fractToggle;
    }

    void OnKeyPress(int key)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(&window, GLFW_TRUE);
            break;
        case GLFW_KEY_SPACE:
            fData.fractToggle = !fData.fractToggle;
            break;
        case GLFW_KEY_F:
            toggleFullscreen(window);
            break;
        default:
            break;
        }
    }

    static void updateRes(float ts)
    {
        if (glfwGetKey(&window, GLFW_KEY_E) == GLFW_PRESS)
            fData.zoom /= 1.0f + ts;
        else if (glfwGetKey(&window, GLFW_KEY_Q) == GLFW_PRESS)
            fData.zoom *= 1.0f + ts;

        fData.res = 2.0f / std::min(window.Size.x, window.Size.y) * fData.zoom;
    }

    static void updateStart(float ts)
    {
        static vec2 startOffset{ 0.0f, 0.0f };
        float offset = 500.0f * fData.res * ts;

        if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS)
            startOffset.y += offset;
        else if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS)
            startOffset.y -= offset;
        if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS)
            startOffset.x -= offset;
        else if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS)
            startOffset.x += offset;

        vec2 mouseDelta = getMousePosDelta(window);

        if (glfwGetMouseButton(&window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            startOffset -= mouseDelta * fData.res;

        fData.start = (vec2)window.Size * -0.5f * fData.res + startOffset;
    }

    static void updateN()
    {
        float f = std::log10(1 / fData.zoom) / 7.0f;
        fData.n = 50 + uint32_t(std::clamp(f, 0.0f, 1.0f) * 150);
    }

}