#ifndef PBD_X_POINTMASS_H
#define PBD_X_POINTMASS_H

#include "Vector3D.h"
#include "Spring.h"
#include <vector>

class PointMass {
public:
    PointMass(float mass, float x, float y, float z);

    void update(float dt);
    void applyForce(const Vector3D& force);

    [[nodiscard]] Vector3D getPosition() const { return position; }
    [[nodiscard]] Vector3D getVelocity() const { return velocity; }
    [[nodiscard]] Vector3D getAcceleration() const { return acceleration; }
    [[nodiscard]] float getMass() const { return mass; }
    [[nodiscard]] bool isFixed() const { return fixed; }

    void setFixed(bool fixed) { this->fixed = fixed; }
    void setPosition(const Vector3D& pos) { position = pos; }
    void setVelocity(const Vector3D& vel) { velocity = vel; }

    void addSpring(Spring* spring);
    [[nodiscard]] const std::vector<Spring*>& getSprings() const { return springs; }

private:
    float mass;
    Vector3D position;
    Vector3D velocity;
    Vector3D acceleration;
    bool fixed;
    std::vector<Spring*> springs;
};


#endif //PBD_X_POINTMASS_H
