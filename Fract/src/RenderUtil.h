#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <vector>

struct RenderData {
    GLuint program, vao, vbo, ibo;
};

inline void setupDraw(RenderData& d)
{
    glGenVertexArrays(1, &d.vao);
    glBindVertexArray(d.vao);

    float vertices[4][2] =
    {
        {  1.0f, -1.0f },
        { -1.0f, -1.0f },
        { -1.0f,  1.0f },
        {  1.0f,  1.0f }
    };
    glGenBuffers(1, &d.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, d.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*) 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
    glGenBuffers(1, &d.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

inline void cleanup(RenderData& d)
{
    glDeleteBuffers(1, &d.vbo);
    glDeleteBuffers(1, &d.ibo);
    glDeleteVertexArrays(1, &d.vao);
    glDeleteProgram(d.program);
}

inline GLuint createShader(GLenum type, const char* shaderSrc)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    GLint status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        std::vector<GLchar> infoLog(length);
        glGetShaderInfoLog(shader, length, &length, &infoLog[0]);
        glDeleteShader(shader);

        fprintf(stderr, "%s\n", infoLog.data());
    }

    return shader;
}


inline GLuint createProgram()
{
    const char* vertexShaderSrc =
#include "../assets/shaders/shader.vert"
        ;
    const char* fragmentShaderSrc =
#include "../assets/shaders/shader.frag"
        ;

    std::vector<GLuint> shaders;
    shaders.push_back(createShader(GL_VERTEX_SHADER, vertexShaderSrc));
    shaders.push_back(createShader(GL_FRAGMENT_SHADER, fragmentShaderSrc));

    GLuint program = glCreateProgram();

    for (GLuint shader : shaders)
        glAttachShader(program, shader);

    glLinkProgram(program);

    for (GLuint shader : shaders)
        glDetachShader(program, shader);

    std::for_each(shaders.begin(), shaders.end(), glDeleteShader);

    return program;
}
