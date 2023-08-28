R"(
#version 330

uniform vec2 start;
uniform float res;
uniform uint maxIt;

uniform int theme;
uniform vec3 color[2];

uniform bool isMandelbrot;

layout(location = 0) out vec4 outColor;

void main()
{
    float v = isMandelbrot ? 2.0 : -2.0;

    vec2 z = vec2(0.0, 0.0);
    vec2 c = start + gl_FragCoord.xy * res;
    uint i = 0u;

    while (z.x * z.x + z.y * z.y < 4.0)
    {
        if (i++ >= maxIt)
            discard;
        z = vec2(z.x * z.x - z.y * z.y + c.x, v * z.x * z.y + c.y);
    }

    switch (theme)
    {
        case 0:
            outColor = vec4(fract(i * color[0].r), fract(i * color[0].g), fract(i * color[0].b), 1.0);
            break;
        case 1:
            outColor = vec4(color[0] + (float(i) / float(maxIt)) * (color[1] - color[0]), 1.0);
            break;
        case 2:
            float t = i + 1.0 - log(log2(length(z)));
            outColor = vec4(fract(t * color[0].r), fract(t * color[0].g), fract(t * color[0].b), 1.0);
            break;
        default:
            outColor = vec4(0.0, 0.0, 0.0, 1.0);
            break;
    }
}
)"
