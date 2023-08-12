#pragma once

namespace Fract {

    template<typename T>
    struct Vec2
    {
        T x, y;

        Vec2<T> operator+(Vec2<T> other) {return {x + other.x, y + other.y};}
        Vec2<T> operator-(Vec2<T> other) {return {x - other.x, y - other.y};}
        Vec2<T> operator*(float scalar) {return {x * scalar, y * scalar};}

        Vec2<T>& operator+=(Vec2<T> other) {x += other.x; y += other.y; return *this;}
        Vec2<T>& operator-=(Vec2<T> other) {x -= other.x; y -= other.y; return *this;}

        T* operator&() {return &x;}
    };

}
