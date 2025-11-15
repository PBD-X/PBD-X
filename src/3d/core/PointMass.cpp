#include "PointMass.h"

PointMass::PointMass(float mass, float x, float y, float z)
    : mass(mass), position(x, y, z), velocity(0, 0, 0), acceleration(0, 0, 0), fixed(false) {
}

void PointMass::update(float dt) {
    if (fixed) return;

    velocity = velocity + acceleration * dt;
    position = position + velocity * dt;

    acceleration = Vector3D(0, 0, 0);
}

void PointMass::applyForce(const Vector3D& force) {
    acceleration = acceleration + force / mass;
}

void PointMass::addSpring(Spring* spring) {
    springs.push_back(spring);
}
