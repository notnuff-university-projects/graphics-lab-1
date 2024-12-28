#version 330 core
in vec2 TexCoords;
in vec2 ScreenPos;
out vec4 color;

uniform sampler2D text;
uniform float time;
uniform float scale;

void main()
{
    // Sample the glyph texture
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);

    // Create a stretched 45-degree rainbow effect using screen position
    float diagonalPosition = (ScreenPos.x + ScreenPos.y) * scale; // Apply scale factor
    float r = 0.5 + 0.5 * sin(time + diagonalPosition);
    float g = 0.5 + 0.5 * sin(time + diagonalPosition + 2.0);
    float b = 0.5 + 0.5 * sin(time + diagonalPosition + 4.0);

    // Apply the scaled diagonal rainbow color to the text
    color = vec4(r, g, b, 1.0) * sampled;
}
