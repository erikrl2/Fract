#pragma once

#include "FractUtil.h"
#include "WindowUtil.h"
#include "RenderUtil.h"

#include <glad/glad.h>

#include <string>
#include <unordered_map>

namespace Fract {

    extern Window window;

    void Init();
    void Update(float ts);
    void UpdateImGui(float ts);

    void Draw(const std::unordered_map<std::string, GLint>& uniformLocations);

    void OnMouseScroll(double offset);
    void OnMouseButtonPress(int button);
    void OnKeyPress(int key);

}
