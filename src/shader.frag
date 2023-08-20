R"(
#version 330

uniform vec2 start;
uniform float res;
uniform uint n;

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

    gl_FragColor = vec4(col(i * 8.0), col(i * 16.0), col(i * 32.0), 1.0);
}
)"
