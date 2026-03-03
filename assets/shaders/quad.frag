#version 330 core

out vec4 fragColor;
in vec2 texCoord;

uniform sampler2D tex;
uniform vec4 color;
uniform vec4 borderColor;
uniform float borderSize;   // in pixels
uniform vec2 texelSize;     // 1.0 / texture resolution

void main()
{
    vec4 texColor = texture(tex, texCoord);

    // If current pixel is visible → draw normally
    if (texColor.a > 0.0)
    {
        fragColor = texColor * color;
        return;
    }

    // Otherwise check surrounding pixels
    float alpha = 0.0;

    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            vec2 offset = vec2(x, y) * texelSize * borderSize;
            alpha = max(alpha, texture(tex, texCoord + offset).a);
        }
    }

    if (alpha > 0.0)
        fragColor = borderColor;
    else
        fragColor = vec4(0.0);
}