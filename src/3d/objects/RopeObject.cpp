#include "RopeObject.h"

RopeObject::RopeObject(float startX, float startY, float startZ, int numPoints, float spacing) {
    PointMass* prev = nullptr;

    for (int i = 0; i < numPoints; i++) {
        PointMass* pm = new PointMass(1.0f, startX, startY + i * spacing, startZ);

        if (i == 0) {
            pm->setFixed(true);
        }

        pointMasses.push_back(pm);

        if (prev != nullptr) {
            springs.push_back(new Spring(prev, pm, 200.0f, 2.0f));
        }

        prev = pm;
    }
}

RopeObject::~RopeObject() {
    for (PointMass* pm : pointMasses) {
        delete pm;
    }
    for (Spring* spring : springs) {
        delete spring;
    }
}

void RopeObject::setStartFixed(bool fixed) {
    if (!pointMasses.empty()) {
        pointMasses[0]->setFixed(fixed);
    }
}
