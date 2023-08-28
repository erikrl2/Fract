#include "Window.h"
#include "Render.h"
#include "Fract.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

int main(int argc, char* argv[])
{
    using namespace Fract;

    FractApp* app = new FractApp("Fract", { 1280, 720 });

    app->Init();

    const Window& window = app->GetWindow();
    while (!glfwWindowShouldClose(&window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float dt = window.GetDeltaTime();
        app->Update(dt);
        app->UpdateImGui(dt);

        app->Draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(&window);
    }

    delete app;

    return EXIT_SUCCESS;
}
