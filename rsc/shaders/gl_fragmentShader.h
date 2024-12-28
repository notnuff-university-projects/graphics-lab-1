//
// Created by notnuff on 27.07.24.
//

#ifndef OPENGL_LEARN_GL_FRAGMENTSHADER_H
#define OPENGL_LEARN_GL_FRAGMENTSHADER_H

inline const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 0.5f);
}
)";

inline const char *fragmentShaderSource2 = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(0.2f, 0.5f, 0.2f, 0.5f);
}
)";

#endif //OPENGL_LEARN_GL_FRAGMENTSHADER_H
