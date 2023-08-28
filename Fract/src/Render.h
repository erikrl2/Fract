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
        GLuint fbo;
        GLuint texture;

        LocationMap locations;
    public:
        RenderData();
        ~RenderData();
    private:
        void CreateProgram();
        void SetupVertexArray();

        GLuint CreateShader(GLenum type, const char* shaderSrc);
    };

}
