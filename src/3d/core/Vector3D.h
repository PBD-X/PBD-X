#ifndef PBD_X_VECTOR3D_H
#define PBD_X_VECTOR3D_H

#include <cmath>

class Vector3D {
public:
    float x, y, z;

    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3D operator+(const Vector3D &other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vector3D operator-(const Vector3D &other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vector3D operator*(float scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }

    Vector3D operator/(float scalar) const {
        return {x / scalar, y / scalar, z / scalar};
    }

    Vector3D &operator+=(const Vector3D &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3D &operator-=(const Vector3D &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3D &operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3D &operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    [[nodiscard]] float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    [[nodiscard]] Vector3D normalized() const {
        float mag = magnitude();
        if (mag > 0) return {x/mag, y/mag, z/mag};
        return {0, 0, 0};
    }

    [[nodiscard]] float dot(const Vector3D &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    [[nodiscard]] Vector3D cross(const Vector3D &other) const {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }

    static float distance(const Vector3D &a, const Vector3D &b) {
        return (b - a).magnitude();
    }
};

#endif //PBD_X_VECTOR3D_H
