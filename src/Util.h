#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <algorithm>

static GLFWwindow* createWindow(const char* title, int width, int height, bool vsync = true)
{
    glfwSetErrorCallback([](int error, const char* description) {
        fprintf(stderr, "Error: %s\n", description);
    });

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowSizeLimits(window, 200, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(vsync);

    return window;
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
#include "shader.vert"
		;
	const char* fragmentShaderSrc =
#include "shader.frag"
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
