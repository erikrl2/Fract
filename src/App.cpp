#include "App.h"

#include <algorithm>
#include <cmath>

namespace Fract {

	using vec2 = Vec2<float>;
	using ivec2 = Vec2<int>;

	static vec2 start;
	static uint32_t n;
	static float res, zoom = 1;
	static bool fractToggle = false;

	void Update(float ts)
	{
		ivec2 size{};
		glfwGetFramebufferSize(window, &size.x, &size.y);

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			zoom /= 1.0f + ts;
		else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			zoom *= 1.0f + ts;

		static vec2 startOffset{ 0.0f, 0.0f };

		res = 2.0f / std::min(size.x, size.y) * zoom;
		float offset = 500.0f * res * ts;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			startOffset.y += offset;
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			startOffset.y -= offset;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			startOffset.x -= offset;
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			startOffset.x += offset;

		static bool pressed = false;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			vec2 mousePos{ float(xpos), size.y - float(ypos) };
			static vec2 lastMousePos;

			if (pressed)
				startOffset -= (mousePos - lastMousePos) * res;
			else
				pressed = true;

			lastMousePos = mousePos;
		}
		else
			pressed = false;

		start = (vec2)size * -0.5f * res + startOffset;

		float f = std::log10(1 / zoom) / 7.0f;
		n = 50 + uint32_t(std::clamp(f, 0.0f, 1.0f) * 150);
	}

	void OnMouseScroll(double offset)
	{
		if (offset > 0.0)
			zoom /= 1.2f;
		else if (offset < 0.0)
			zoom *= 1.2f;
	}

	void OnMouseButtonPress(int button)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			fractToggle = !fractToggle;
	}

	void OnKeyPress(int key)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		}
		case GLFW_KEY_SPACE:
		{
			fractToggle = !fractToggle;
			break;
		}
		case GLFW_KEY_F:
		{
			static bool fullscreen = true;
			static ivec2 pos, size;
			if (fullscreen)
			{
				GLFWmonitor* monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);
				glfwGetWindowPos(window, &pos.x, &pos.y);
				glfwGetWindowSize(window, &size.x, &size.y);
				glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
			}
			else
			{
				glfwSetWindowMonitor(window, NULL, pos.x, pos.y, size.x, size.y, GLFW_DONT_CARE);
			}
			fullscreen = !fullscreen;
			break;
		}
		default:
		{
			break;
		}
		}
	}

	void Draw(const std::unordered_map<std::string, GLint>& uniformLocations)
	{
		glUniform2fv(uniformLocations.at("start"), 1, &start);
		glUniform1f(uniformLocations.at("res"), res);
		glUniform1ui(uniformLocations.at("n"), n);
		glUniform1i(uniformLocations.at("mandelb"), fractToggle);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

}