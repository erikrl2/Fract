#pragma once

#include <string>

inline std::string vertexShaderSrc = R"(
#version 400

layout(location = 0) in vec2 pos;

void main()
{
    gl_Position = vec4(pos, 0.0, 1.0);
}
)";

inline std::string fragmentShaderSrc = R"(
#version 400

uniform dvec2 start;
uniform double res;
uniform uint n;

uniform bool mandelb;

float col(float x)
{
    return fract(x / 256.0);
}

void main()
{
    double v = mandelb ? 2.0lf : -2.0lf;

    dvec2 z = dvec2(0.0lf, 0.0lf);
    dvec2 c = start + gl_FragCoord.xy * res;
    uint i = 0u;

    while (z.x * z.x + z.y * z.y < 4.0lf)
    {
        if (i++ >= n)
            discard;
        z = dvec2(z.x * z.x - z.y * z.y + c.x, v * z.x * z.y + c.y);
    }

    gl_FragColor = vec4(col(i * 8.0f), col(i * 16.0f), col(i * 32.0f), 1.0f);
}
)";
