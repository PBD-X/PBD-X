//
// Created by Kevin Zheng on 11/5/2025.
//

#ifndef PBD_X_VECTOR2_H
#define PBD_X_VECTOR2_H

#include <cmath>

class Vector2 {
public:
    float x,y;

    Vector2() : x(0), y(0) {}
    Vector2(float _x, float _y) : x(_x), y(_y) {}

    Vector2 operator+(const Vector2 &other) const {
        return {x + other.x, y + other.y};
    }

    Vector2 operator-(const Vector2 &other) const {
        return {x - other.x, y - other.y};
    }

    Vector2 operator*(float scalar) const {
        return {x * scalar, y * scalar};
    }

    Vector2 operator/(float scalar) const {
        return {x / scalar, y / scalar};
    }

    Vector2& operator+=(const Vector2 &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& operator-=(const Vector2 &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    [[nodiscard]] float dot(const Vector2 &other) const {
        return x * other.x + y * other.y;
    }

    [[nodiscard]] float length() const {
        return sqrtf(x * x + y * y);
    }

    [[nodiscard]] float lengthSquared() const {
        return x * x + y * y;
    }

    [[nodiscard]] Vector2 normalized() const {
        float len = length();
        if (len > 0) {
            return *this / len;
        }
        return *this;
    }

    void normalize() {
        float len = length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
    }

    [[nodiscard]] float distance(const Vector2 &other) const {
        return (*this - other).length();
    }

    [[nodiscard]] float distanceSquared(const Vector2 &other) const {
        return (*this - other).lengthSquared();
    }
};

inline Vector2 operator*(float scalar, const Vector2 &vec) {
    return vec * scalar;
}

#endif //PBD_X_VECTOR2_H