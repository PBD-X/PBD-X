//
// Created by Kevin Zheng on 11/9/2025.
//

#ifndef PBD_X_POINTMASS_H
#define PBD_X_POINTMASS_H

#include "Vector2D.h"
#include "Spring.h"
#include <vector>

class PointMass {
public:
    PointMass(float mass, float x, float y);

    void update(float dt);
    void applyForce(const Vector2D& force);

    [[nodiscard]] Vector2D getPosition() const { return position; }
    [[nodiscard]] Vector2D getVelocity() const { return velocity; }
    [[nodiscard]] Vector2D getAcceleration() const { return acceleration; }
    [[nodiscard]] float getMass() const { return mass; }
    [[nodiscard]] bool isFixed() const { return fixed; }

    void setFixed(bool fixed) { this->fixed = fixed; }
    void setPosition(const Vector2D& pos) { position = pos; }
    void setVelocity(const Vector2D& vel) { velocity = vel; }

    void addSpring(Spring* spring);
    [[nodiscard]] const std::vector<Spring*>& getSprings() const { return springs; }

private:
    float mass;
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
    bool fixed;
    std::vector<Spring*> springs;
};


#endif //PBD_X_POINTMASS_H