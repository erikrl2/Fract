#include "Fract.h"

#include "Window.h"
#include "Render.h"
#include "Util.h"

#include <imgui/imgui.h>

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace Fract {

    FractApp::FractApp(const std::string& title, IVec2 size, bool vSync)
        : window(title, size, vSync)
    {
        glfwSetWindowUserPointer(&window, (void*) this);
    }

    FractApp::~FractApp()
    {
    }

    void FractApp::Init()
    {
        fData.cycleColor = { 0.03125f, 0.0625f, 0.125f };
        fData.lerpColors[0] = { 0, 0, 0 };
        fData.lerpColors[1] = { 1, 1, 1 };
        fData.smoothColor = { 0.125f, 0, 0 };

        rData.locations["start"] = glGetUniformLocation(rData.program, "start");
        rData.locations["res"] = glGetUniformLocation(rData.program, "res");
        rData.locations["maxIt"] = glGetUniformLocation(rData.program, "maxIt");
        rData.locations["theme"] = glGetUniformLocation(rData.program, "theme");
        rData.locations["color"] = glGetUniformLocation(rData.program, "color");
        rData.locations["isMandelbrot"] = glGetUniformLocation(rData.program, "isMandelbrot");

        glUseProgram(rData.program);
        glBindVertexArray(rData.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rData.ibo);
    }

    void FractApp::Update(float ts)
    {
        UpdateRes(ts);
        UpdateStart(ts);
        UpdateMaxIt(ts);
    }

    void FractApp::UpdateImGui(float ts)
    {
        using namespace ImGui;

        if (!fData.showSettings)
            return;

        SetNextWindowPos({});
        SetNextWindowSize({ 325, 0 });

        Begin("Settings", &fData.showSettings, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNavInputs);

        if (Checkbox("VSync", &window.vSync))
            glfwSwapInterval(window.vSync);

        SameLine(0, 40);
        float dtAvg = GetDeltaTimeAverage(ts);
        Text("%.2f ms (%u FPS)", (double) dtAvg * 1000, (uint32_t) (1 / dtAvg));

        SameLine();
        SetCursorPos({ 285, 1 });
        if (SmallButton("Hide"))
            fData.showSettings = false;
        if (BeginItemTooltip())
        {
            TextUnformatted("Toggle with Spacebar");
            EndTooltip();
        }

        BeginDisabled(fData.fullscreen);
        DragInt2("Window Size", &window.size, 1, 0, 0, "%d", 0);
        glfwSetWindowSize(&window, window.size.x, window.size.y);
        EndDisabled();

        if (Checkbox("Fullscreen", &fData.fullscreen))
            SetFullscreen(fData.fullscreen);

        SeparatorText("Fractal");

        const char* fractals[] = { "Tricorn", "Mandelbrot" };
        Combo("Fractal", (int*) & fData.fractalSelection, fractals, sizeof fractals / sizeof fractals[0]);

        DragFloat2("Position", &fData.start, 2.0f * fData.resolution, 0, 0, "%.5f", ImGuiSliderFlags_NoRoundToFormat);

        SliderFloat("Zoom", &fData.zoom, 0.5f, 2e4f, "", ImGuiSliderFlags_Logarithmic);

        Checkbox("Iteration Limit", &fData.customMaxIt);
        SameLine();
        BeginDisabled(!fData.customMaxIt);
        SetNextItemWidth(75);
        DragInt("##maxIterations", (int*)&fData.maxIterations, 1, 1, 500, "%d", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
        EndDisabled();

        SeparatorText("Theme");

        ColorEdit3("Center", &fData.clearColor);

        const char* themes[] = { "Cycle", "Lerp", "Smooth" };
        Combo("Theme", (int*) &fData.themeSelection, themes, sizeof themes / sizeof themes[0]);

        switch (fData.themeSelection)
        {
        case Theme::Cycle:
        {
            ColorEdit3("##Cycle", &fData.cycleColor);
            fData.themeColors[0] = fData.cycleColor;
            break;
        }
        case Theme::Lerp:
        {
            ColorEdit3("From##Lerp", &fData.lerpColors[0]);
            ColorEdit3("To##Lerp", &fData.lerpColors[1]);
            fData.themeColors[0] = fData.lerpColors[0];
            fData.themeColors[1] = fData.lerpColors[1];
            break;
        }
        case Theme::Smooth:
        {
            ColorEdit3("##Smooth", &fData.smoothColor);
            fData.themeColors[0] = fData.smoothColor;
            break;
        }
        default:
        {
            fprintf(stderr, "Theme %d not supported", (int) fData.themeSelection);
            break;
        }
        }

        SeparatorText("Saving");

        if (Button("Save Image"))
        {
            // TODO: Open save dialog
        }

        SameLine(300);
        TextDisabled("(?)");
        if (BeginItemTooltip())
        {
            static const char* helpText = "Panning:\tW/A/S/D or Mouse drag\nZooming:\tE/Q or Mouse scroll\nFullscreen: F";
            TextUnformatted(helpText);
            EndTooltip();
        }

        End();
    }

    void FractApp::Draw()
    {
        const Color& cc = fData.clearColor;
        glClearColor(cc.R, cc.G, cc.B, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform2fv(rData.locations["start"], 1, &fData.startInternal);
        glUniform1f(rData.locations["res"], fData.resolution);
        glUniform1ui(rData.locations["maxIt"], fData.maxIterations);
        glUniform1i(rData.locations["theme"], (int) fData.themeSelection);
        glUniform3fv(rData.locations["color"], 2, &fData.themeColors[0]);
        glUniform1i(rData.locations["isMandelbrot"], (int) fData.fractalSelection);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void FractApp::OnMouseScroll(double offset)
    {
        if (ImGui::GetIO().WantCaptureMouse)
            return;

        if (offset > 0.0)
            fData.zoom *= 1.2f;
        else if (offset < 0.0)
            fData.zoom /= 1.2f;
    }

    void FractApp::OnMouseButtonPress(int button)
    {
        if (ImGui::GetIO().WantCaptureMouse)
            return;
    }

    void FractApp::OnKeyPress(int key)
    {
        if (ImGui::GetIO().WantTextInput)
            return;

        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(&window, GLFW_TRUE);
            break;
        case GLFW_KEY_SPACE:
            fData.showSettings = !fData.showSettings;
            break;
        case GLFW_KEY_F:
            fData.fullscreen = !fData.fullscreen;
            SetFullscreen(fData.fullscreen);
            break;
        default:
            break;
        }
    }

    void FractApp::UpdateRes(float ts)
    {
        if (!ImGui::GetIO().WantTextInput)
        {
            if (glfwGetKey(&window, GLFW_KEY_E) == GLFW_PRESS)
                fData.zoom *= 1.0f + ts;
            else if (glfwGetKey(&window, GLFW_KEY_Q) == GLFW_PRESS)
                fData.zoom /= 1.0f + ts;
        }

        fData.resolution = 2.0f / (std::min(window.size.x, window.size.y) * fData.zoom);
    }

    void FractApp::UpdateStart(float ts)
    {
        if (!ImGui::GetIO().WantTextInput)
        {
            float offset = 500.0f * fData.resolution * ts;
            if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS)
                fData.start.y += offset;
            else if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS)
                fData.start.y -= offset;
            if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS)
                fData.start.x -= offset;
            else if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS)
                fData.start.x += offset;
        }

        if (!ImGui::GetIO().WantCaptureMouse)
        {
            Vec2 mouseDelta = GetMousePosDelta();
            if (glfwGetMouseButton(&window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                fData.start -= mouseDelta * fData.resolution;
            }
        }

        fData.startInternal = (Vec2)window.size * -0.5f * fData.resolution + fData.start;
    }

    void FractApp::UpdateMaxIt(float ts)
    {
        if (!fData.customMaxIt)
            fData.maxIterations = 50 + uint32_t(std::clamp(std::log10(fData.zoom) / 7.0f, 0.0f, 1.0f) * 150);
    }

    void FractApp::SetFullscreen(bool fullscreen)
    {
        static IVec2 pos, size;

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

        glfwSwapInterval((int)window.vSync);
    }

    Vec2 FractApp::GetMousePosDelta()
    {
        double xpos, ypos;
        glfwGetCursorPos(&window, &xpos, &ypos);

        Vec2 mousePos{ float(xpos), window.size.y - float(ypos) };
        static Vec2 lastMousePos = mousePos;

        Vec2 mouseDelta = mousePos - lastMousePos;

        lastMousePos = mousePos;

        return mouseDelta;
    }

    float FractApp::GetDeltaTimeAverage(float dt)
    {
        static int index = 0;
        static float sum = 0.0f;
        static float list[100]{};

        sum -= list[index];
        sum += dt;
        list[index] = dt;
        index = (index + 1) % 100;

        return sum / 100.0f;
    }

}
