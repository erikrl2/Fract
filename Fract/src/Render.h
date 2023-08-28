#pragma once

#include <glad/glad.h>

#include <unordered_map>
#include <string>

namespace Fract {

    using LocationMap = std::unordered_map<std::string, GLuint>;

    class RenderData {
    public:
        GLuint program;
        GLuint vao, vbo, ibo;

        LocationMap locations;
    public:
        RenderData();
        ~RenderData();
    private:
        GLuint CreateProgram();
        GLuint CreateShader(GLenum type, const char* shaderSrc);
    };

}
