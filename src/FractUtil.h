#pragma once

#include <GLFW/glfw3.h>

#include <cstdint>

namespace Fract {

	template<typename T>
	struct Vec2
	{
		T x, y;

		Vec2<T> operator+(Vec2<T> other) { return { x + other.x, y + other.y }; }
		Vec2<T> operator-(Vec2<T> other) { return { x - other.x, y - other.y }; }
		Vec2<T> operator*(float scalar) { return { x * scalar, y * scalar }; }

		Vec2<T>& operator+=(Vec2<T> other) { x += other.x; y += other.y; return *this; }
		Vec2<T>& operator-=(Vec2<T> other) { x -= other.x; y -= other.y; return *this; }

		T* operator&() { return &x; }

		template<typename U>
		operator Vec2<U>() { return { (U)x, (U)y }; }
	};

	typedef Vec2<float> vec2;
	typedef Vec2<int> ivec2;

	struct FractData
	{
		vec2 start;
		uint32_t n;
		float res, zoom = 1;
		bool fractToggle = false;
	};

	inline void toggleFullscreen(GLFWwindow* window)
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
	}

}
