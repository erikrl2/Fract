#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>

namespace Fract {

	extern GLFWwindow* window;

	void Update(float ts);
	void Draw(const std::unordered_map<std::string, GLint>& uniformLocations);

	void OnMouseScroll(double offset);
	void OnMouseButtonPress(int button);
	void OnKeyPress(int key);

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
}
