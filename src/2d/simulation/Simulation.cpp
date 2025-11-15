//
// Created by Kevin Zheng on 11/9/2025.
//

#include "Simulation.h"
#include <iostream>

Simulation::Simulation() {
}

Simulation::~Simulation() {
    clear();
}

void Simulation::update(float dt) {
    for (Spring* spring : springs) {
        spring->applyForces();
    }

    for (PointMass* pm : pointMasses) {
        pm->update(dt);
    }
}

void Simulation::addPointMass(PointMass* pointMass) {
    pointMasses.push_back(pointMass);
}

void Simulation::addSpring(Spring* spring) {
    springs.push_back(spring);
}

void Simulation::createCloth(float startX, float startY, int width, int height, float spacing) {
    std::vector<std::vector<PointMass*>> grid(height, std::vector<PointMass*>(width));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            PointMass* pm = new PointMass(1.0f, startX + x * spacing, startY + y * spacing);

            if (y == 0 && (x == 0 || x == width - 1)) {
                pm->setFixed(true);
            }

            grid[y][x] = pm;
            addPointMass(pm);
        }
    }

    float stiffness = 100.0f;
    float damping = 1.0f;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x < width -1) {
                addSpring(new Spring(grid[y][x], grid[y][x+1], stiffness, damping));
            }
            if (y < height - 1) {
                addSpring(new Spring(grid[y][x], grid[y+1][x], stiffness, damping));
            }

            if (x < width - 1 && y < height - 1) {
                addSpring(new Spring(grid[y][x], grid[y+1][x+1], stiffness * 0.3f, damping));
                addSpring(new Spring(grid[y][x+1], grid[y+1][x], stiffness * 0.3f, damping));
            }

            if (x < width - 2) {
                addSpring(new Spring(grid[y][x], grid[y][x+2], stiffness * 0.2f, damping));
            }
            if (y < height - 2) {
                addSpring(new Spring(grid[y][x], grid[y+2][x], stiffness * 0.2f, damping));
            }
        }
    }
}

void Simulation::createRope(float startX, float startY, int numPoints, float spacing) {
    PointMass* prev = nullptr;

    for (int i = 0; i < numPoints; i++) {
        PointMass* pm = new PointMass(1.0f, startX, startY + i * spacing);

        if (i == 0) {
            pm->setFixed(true);
        }

        addPointMass(pm);

        if (prev != nullptr) {
            addSpring(new Spring(prev, pm, 200.0f, 2.0f));
        }

        prev = pm;
    }
}

void Simulation::clear() {
    for (PointMass* pm : pointMasses) {
        delete pm;
    }
    for (Spring* spring : springs) {
        delete spring;
    }
    pointMasses.clear();
    springs.clear();
}

void Simulation::applyGlobalForce(const Vector2D& force) {
    for (PointMass* pm : pointMasses) {
        if (!pm->isFixed()) {
            pm->applyForce(force);
        }
    }
}
