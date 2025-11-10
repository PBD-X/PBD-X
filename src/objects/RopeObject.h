//
// Created by Kevin Zheng on 11/9/2025.
//

#ifndef PBD_X_ROPEOBJECT_H
#define PBD_X_ROPEOBJECT_H

#include <vector>
#include "../core/PointMass.h"
#include "../core/Spring.h"

class RopeObject {
public:
    RopeObject(float startX, float startY, int numPoints, float spacing);
    ~RopeObject();

    [[nodiscard]] const std::vector<PointMass*>& getPointMasses() const { return pointMasses; }
    [[nodiscard]] const std::vector<Spring*>& getSprings() const { return springs; }

    void setStartFixed(bool fixed);

private:
    std::vector<PointMass*> pointMasses;
    std::vector<Spring*> springs;
};


#endif //PBD_X_ROPEOBJECT_H