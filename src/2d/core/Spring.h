//
// Created by Kevin Zheng on 11/9/2025.
//

#ifndef PBD_X_SPRING_H
#define PBD_X_SPRING_H

class PointMass;


class Spring {
public:
    Spring(PointMass* p1, PointMass* p2, float stiffness, float damping, float restLength = -1.0f);

    void applyForces();

    [[nodiscard]] PointMass* getPointMass1() const { return pointMass1; }
    [[nodiscard]] PointMass* getPointMass2() const { return pointMass2; }
    [[nodiscard]] float getRestLength() const { return restLength; }
    [[nodiscard]] float getStiffness() const { return stiffness; }
    [[nodiscard]] float getDamping() const { return damping; }
    [[nodiscard]] float getCurrentLength() const;

private:
    PointMass* pointMass1;
    PointMass* pointMass2;
    float stiffness;
    float damping;
    float restLength;
};


#endif //PBD_X_SPRING_H
