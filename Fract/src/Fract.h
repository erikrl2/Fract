#pragma once

#include "Window.h"
#include "Render.h"

namespace Fract {

    enum class Fractal { Tricorn, Mandelbrot };
    enum class Theme { Cycle, Lerp, Smooth };

    struct FractData
    {
        Vec2 start, startInternal;
        uint32_t maxIterations = 50;
        float resolution{}, zoom = 1;

        Fractal fractalSelection = Fractal::Tricorn;

        Theme themeSelection = Theme::Cycle;
        Color clearColor;
        Color themeColors[2];

        Color cycleColor;
        Color lerpColors[2];
        Color smoothColor;

        bool showSettings = true;
        bool fullscreen = false;
        bool customMaxIt = false;
    };

    class FractApp
    {
    public:
        FractApp(const std::string& title, IVec2 size, bool vSync = true);
        ~FractApp();

        void Update(float ts);
        void UpdateImGui(float ts);

        void Draw();

        void OnMouseScroll(double offset);
        void OnMouseButtonPress(int button);
        void OnKeyPress(int key);
    private:
        void UpdateStart(float ts);
        void UpdateRes(float ts);
        void UpdateMaxIt(float ts);

        void SaveImage();
        void SetFullscreen(bool fullscreen = true);
        float GetDeltaTimeAverage(float dt);
        Vec2 GetMousePosDelta();
    public:
        const Window& GetWindow() const { return window; }

        void SetWindowSize(IVec2 size) { window.size = size; }
    private:
        Window window;
        RenderData rData;
        FractData fData;
    };

}
