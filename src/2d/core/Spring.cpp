//
// Created by Kevin Zheng on 11/9/2025.
//

#include "Spring.h"
#include "PointMass.h"

Spring::Spring(PointMass* p1, PointMass* p2, float stiffness, float damping, float restLength)
    : pointMass1(p1), pointMass2(p2), stiffness(stiffness), damping(damping) {

    if (restLength < 0) {
        Vector2D delta = p2->getPosition() - p1->getPosition();
        this->restLength = delta.magnitude();
    } else {
        this-> restLength = restLength;
    }

    p1->addSpring(this);
    p2->addSpring(this);
}

void Spring::applyForces() {
    Vector2D p1 = pointMass1->getPosition();
    Vector2D p2 = pointMass2->getPosition();
    Vector2D v1 = pointMass1->getVelocity();
    Vector2D v2 = pointMass2->getVelocity();

    Vector2D delta = p2 - p1;
    float currentLength = delta.magnitude();

    if (currentLength == 0) return;

    float displacement = currentLength - restLength;
    Vector2D springForce = delta.normalized() * (stiffness * displacement);

    Vector2D relativeVelocity = v2 - v1;
    Vector2D dampingForce = delta.normalized() * (damping * relativeVelocity.dot(delta.normalized()));

    Vector2D totalForce = springForce + dampingForce;

    pointMass1->applyForce(totalForce);
    pointMass2->applyForce(totalForce * -1.0f);
}

float Spring::getCurrentLength() const {
    Vector2D delta = pointMass2->getPosition() - pointMass1->getPosition();
    return delta.magnitude();
}
