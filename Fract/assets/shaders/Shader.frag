R"(
#version 330

uniform vec2 start;
uniform float res;
uniform uint n;

uniform int theme;
uniform vec3 colors[2];

uniform bool mandelb;

float col(float x)
{
    return fract(x / 256.0);
}

void main()
{
    float v = mandelb ? 2.0 : -2.0;

    vec2 z = vec2(0.0, 0.0);
    vec2 c = start + gl_FragCoord.xy * res;
    uint i = 0u;

    while (z.x * z.x + z.y * z.y < 4.0)
    {
        if (i++ >= n)
            discard;
        z = vec2(z.x * z.x - z.y * z.y + c.x, v * z.x * z.y + c.y);
    }

    vec3 color;
    switch (theme)
    {
        case 0:
        {
            color = vec3(col(i * 8.0), col(i * 16.0), col(i * 32.0));
            break;
        }
        case 1:
        {
            float t = float(i) / float(n);
            color = colors[0] + t * (colors[1] - colors[0]);
            break;
        }
    }

    gl_FragColor = vec4(color, 1.0);
}
)"
