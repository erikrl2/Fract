#pragma once

namespace Fract {

    template<typename T>
    struct Vector2
    {
        T x, y;

        Vector2() : x(0), y(0) {}
        Vector2(T x, T y) : x(x), y(y) {}

        Vector2<T> operator+(Vector2<T> other) const { return { x + other.x, y + other.y }; }
        Vector2<T> operator-(Vector2<T> other) const { return { x - other.x, y - other.y }; }
        Vector2<T> operator*(float scalar) const { return { x * scalar, y * scalar }; }

        Vector2<T>& operator+=(Vector2<T> other) { x += other.x; y += other.y; return *this; }
        Vector2<T>& operator-=(Vector2<T> other) { x -= other.x; y -= other.y; return *this; }

        T* operator&() { return &x; }

        template<typename U>
        operator Vector2<U>() const { return { (U)x, (U)y }; }
    };

    using Vec2 = Vector2<float>;
    using IVec2 = Vector2<int>;

    struct Color
    {
        float R, G, B;

        Color() : R(0), G(0), B(0) {}
        Color(float r, float g, float b) : R(r), G(g), B(b) {}

        float* operator&() { return &R; }
    };

}
