//
// Created by notnuff on 27.07.24.
//

#ifndef OPENGL_LEARN_GL_VERTEXSHADER_H
#define OPENGL_LEARN_GL_VERTEXSHADER_H

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

#endif //OPENGL_LEARN_GL_VERTEXSHADER_H
