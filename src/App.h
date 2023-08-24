#pragma once

#include "FractUtil.h"

#include <glad/glad.h>

#include <string>
#include <unordered_map>

namespace Fract {

    extern Window window;

    void Update(float ts);
    void Draw(const std::unordered_map<std::string, GLint>& uniformLocations);

    void OnMouseScroll(double offset);
    void OnMouseButtonPress(int button);
    void OnKeyPress(int key);

}
