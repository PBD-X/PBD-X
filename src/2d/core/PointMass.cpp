//
// Created by Kevin Zheng on 11/9/2025.
//

#include "PointMass.h"

PointMass::PointMass(float mass, float x, float y)
    : mass(mass), position(x, y), velocity(0, 0), acceleration(0, 0), fixed(false) {
}

void PointMass::update(float dt) {
    if (fixed) return;

    velocity = velocity + acceleration * dt;
    position = position + velocity * dt;

    acceleration = Vector2D(0, 0);
}

void PointMass::applyForce(const Vector2D& force) {
    acceleration = acceleration + force / mass;
}

void PointMass::addSpring(Spring* spring) {
    springs.push_back(spring);
}
