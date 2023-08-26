#include "App.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <algorithm>
#include <cmath>

namespace Fract {

    static struct Data
    {
        vec2 Start, StartInternal;
        uint32_t N = 50;
        float Res{}, Zoom = 1;
        bool Mandelbrot = false;

        bool ShowSettings = true;
        bool Fullscreen = false;
        bool CustomN = false;
    } fData;

    static void updateStart(float ts);
    static void updateRes(float ts);
    static void updateN();

    void Update(float ts)
    {
        updateRes(ts);
        updateStart(ts);
        updateN();
    }

    void UpdateImGui(float ts)
    {
        using namespace ImGui;

        if (fData.ShowSettings)
        {
            SetNextWindowPos(ImVec2{});
            SetNextWindowSize(ImVec2{ 325, 0 });
            Begin("Settings", &fData.ShowSettings, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNavInputs);

            BeginDisabled(fData.Fullscreen);
            DragInt2("Window Size", &window.Size, 1, 0, 0, "%d", 0);
            glfwSetWindowSize(&window, window.Size.X, window.Size.Y);
            EndDisabled();

            if (Checkbox("Fullscreen", &fData.Fullscreen))
                setFullscreen(window, fData.Fullscreen);

            DragFloat2("Position", &fData.Start, 2.0f * fData.Res, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);

            SliderFloat("Zoom", &fData.Zoom, 0.5f, 3e5f, "", ImGuiSliderFlags_Logarithmic);

            Checkbox("Iteration Limit", &fData.CustomN);
            SameLine();
            BeginDisabled(!fData.CustomN);
            SetNextItemWidth(75);
            DragInt("##N", (int*)&fData.N, 1, 1, 500, "%d", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
            EndDisabled();

            Checkbox("Mandelbrot", &fData.Mandelbrot);

            // TODO: Coloring options

            if (Button("Save Image"))
            {
                // TODO: Open save dialog
            }

            NewLine();
            Text("%.2f ms (%u FPS)", ts * 1000, (uint32_t) (1 / ts));

            SameLine();
            if (Checkbox("VSync", &window.VSync))
                glfwSwapInterval(window.VSync);

            SameLine(300);
            TextDisabled("(?)");
            if (BeginItemTooltip())
            {
                TextUnformatted("Press Space to toggle this window");
                EndTooltip();
            }

            End();
        }
    }

    void Draw(const std::unordered_map<std::string, GLint>& uniformLocations)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform2fv(uniformLocations.at("start"), 1, &fData.StartInternal);
        glUniform1f(uniformLocations.at("res"), fData.Res);
        glUniform1ui(uniformLocations.at("n"), fData.N);
        glUniform1i(uniformLocations.at("mandelb"), fData.Mandelbrot);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void OnMouseScroll(double offset)
    {
        if (ImGui::GetIO().WantCaptureMouse)
            return;

        if (offset > 0.0)
            fData.Zoom *= 1.2f;
        else if (offset < 0.0)
            fData.Zoom /= 1.2f;
    }

    void OnMouseButtonPress(int button)
    {
        if (ImGui::GetIO().WantCaptureMouse)
            return;
    }

    void OnKeyPress(int key)
    {
        if (ImGui::GetIO().WantTextInput)
            return;

        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(&window, GLFW_TRUE);
            break;
        case GLFW_KEY_SPACE:
            fData.ShowSettings = !fData.ShowSettings;
            break;
        case GLFW_KEY_F:
            fData.Fullscreen = !fData.Fullscreen;
            setFullscreen(window, fData.Fullscreen);
            break;
        default:
            break;
        }
    }

    static void updateRes(float ts)
    {
        if (!ImGui::GetIO().WantTextInput)
        {
            if (glfwGetKey(&window, GLFW_KEY_E) == GLFW_PRESS)
                fData.Zoom *= 1.0f + ts;
            else if (glfwGetKey(&window, GLFW_KEY_Q) == GLFW_PRESS)
                fData.Zoom /= 1.0f + ts;
        }

        fData.Res = 2.0f / (std::min(window.Size.X, window.Size.Y) * fData.Zoom);
    }

    static void updateStart(float ts)
    {
        if (!ImGui::GetIO().WantTextInput)
        {
            float offset = 500.0f * fData.Res * ts;
            if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS)
                fData.Start.Y += offset;
            else if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS)
                fData.Start.Y -= offset;
            if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS)
                fData.Start.X -= offset;
            else if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS)
                fData.Start.X += offset;
        }

        if (!ImGui::GetIO().WantCaptureMouse)
        {
            vec2 mouseDelta = getMousePosDelta(window);
            if (glfwGetMouseButton(&window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                fData.Start -= mouseDelta * fData.Res;
            }
        }

        fData.StartInternal = (vec2)window.Size * -0.5f * fData.Res + fData.Start;
    }

    static void updateN()
    {
        if (!fData.CustomN)
            fData.N = 50 + uint32_t(std::clamp(std::log10(fData.Zoom) / 7.0f, 0.0f, 1.0f) * 150);
    }

}
