#pragma once

#include <string>

inline std::string vertexShaderSrc = R"(
#version 330

layout(location = 0) in vec2 pos;

void main()
{
    gl_Position = vec4(pos, 0.0, 1.0);
}
)";

inline std::string fragmentShaderSrc = R"(
#version 330

uniform vec2 start;
uniform float res;
uniform uint n;

float col(float x)
{
    return fract(x / 256.0);
}

void main()
{
    vec2 z = vec2(0.0, 0.0);
    vec2 c = start + gl_FragCoord.xy * res;
    uint i = 0u;

    while (z.x * z.x + z.y * z.y < 4.0)
    {
        if (i++ >= n)
            discard;
        z = vec2(z.x * z.x - z.y * z.y + c.x, -2.0 * z.x * z.y + c.y);
    }

    gl_FragColor = vec4(col(i * 8.0), col(i * 16.0), col(i * 32.0), 1.0);
}
)";
