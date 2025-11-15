#include "Spring.h"
#include "PointMass.h"

Spring::Spring(PointMass* p1, PointMass* p2, float stiffness, float damping, float restLength)
    : pointMass1(p1), pointMass2(p2), stiffness(stiffness), damping(damping) {

    if (restLength < 0) {
        Vector3D delta = p2->getPosition() - p1->getPosition();
        this->restLength = delta.magnitude();
    } else {
        this-> restLength = restLength;
    }

    p1->addSpring(this);
    p2->addSpring(this);
}

void Spring::applyForces() {
    Vector3D p1 = pointMass1->getPosition();
    Vector3D p2 = pointMass2->getPosition();
    Vector3D v1 = pointMass1->getVelocity();
    Vector3D v2 = pointMass2->getVelocity();

    Vector3D delta = p2 - p1;
    float currentLength = delta.magnitude();

    if (currentLength == 0) return;

    float displacement = currentLength - restLength;
    Vector3D springForce = delta.normalized() * (stiffness * displacement);

    Vector3D relativeVelocity = v2 - v1;
    Vector3D dampingForce = delta.normalized() * (damping * relativeVelocity.dot(delta.normalized()));

    Vector3D totalForce = springForce + dampingForce;

    pointMass1->applyForce(totalForce);
    pointMass2->applyForce(totalForce * -1.0f);
}

float Spring::getCurrentLength() const {
    Vector3D delta = pointMass2->getPosition() - pointMass1->getPosition();
    return delta.magnitude();
}
