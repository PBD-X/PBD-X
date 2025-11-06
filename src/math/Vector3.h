//
// Created by Kevin Zheng on 11/5/2025.
//

#ifndef PBD_X_VECTOR3_H
#define PBD_X_VECTOR3_H
#include <cmath>
#include <math.h>

class Vector3 {
public:
    float x, y, z;

    Vector3(): x(0), y(0), z(0) {}
    Vector3(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}

    Vector3 operator+(const Vector3 &other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vector3 operator-(const Vector3 &other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vector3 operator*(float scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }

    Vector3 operator/(float scalar) const {
        return {x / scalar, y / scalar, z / scalar};
    }

    Vector3& operator+=(const Vector3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3& operator-=(const Vector3 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    float dot(const Vector3 &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 cross(const Vector3 &other) const {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }

    float length() const {
        return sqrt(x * x + y * y + z * z);
    }

    float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    Vector3 normalized() const {
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
            z /= len;
        }
    }

    float distance(const Vector3 &other) const {
        return (*this - other).length();
    }

    float distanceSquared(const Vector3 &other) const {
        return (*this - other).lengthSquared();
    }
};

inline Vector3 operator*(float scalar, const Vector3 &vec) {
    return vec * scalar;
}

#endif //PBD_X_VECTOR3_H