//
// Created by Kevin Zheng on 11/9/2025.
//

#include "ClothObject.h"

ClothObject::ClothObject(float startX, float startY, int width, int height, float spacing)
    : width(width), height(height) {

    grid.resize(height, std::vector<PointMass*>(width));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            PointMass* pm = new PointMass(1.0f, startX + x * spacing, startY + y * spacing);
            grid[y][x] = pm;
            pointMasses.push_back(pm);
        }
    }

    setCornersFixed(true);

    createSprings();
}

ClothObject::~ClothObject() {
    for (PointMass*pm : pointMasses) {
        delete pm;
    }
    for (Spring* spring : springs) {
        delete spring;
    }
}

void ClothObject::createSprings() {
    float structuralStiffness = 100.0f;
    float shearStiffness = 30.0f;
    float bendStiffness = 20.0f;
    float damping = 1.0f;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x < width - 1) {
                springs.push_back(new Spring(grid[y][x], grid[y][x+1], structuralStiffness, damping));
            }

            if (y < height - 1) {
                springs.push_back(new Spring(grid[y][x], grid[y+1][x], structuralStiffness, damping));
            }

            if (x < width - 1 && y < height - 1) {
                springs.push_back(new Spring(grid[y][x], grid[y+1][x+1], shearStiffness, damping));
                springs.push_back(new Spring(grid[y][x+1], grid[y+1][x], shearStiffness, damping));
            }

            if (x < width - 2) {
                springs.push_back(new Spring(grid[y][x], grid[y][x+2], bendStiffness, damping));
            }
            if (y < height - 2) {
                springs.push_back(new Spring(grid[y][x], grid[y+2][x], bendStiffness, damping));
            }
        }
    }
}

void ClothObject::setCornersFixed(bool fixed) {
    if (height > 0 && width > 0) {
        grid[0][0]->setFixed(fixed);
        grid[0][width - 1]->setFixed(fixed);
    }
}

void ClothObject::applyWindForce(const Vector2D &force) {
    for (PointMass* pm : pointMasses) {
        if (!pm->isFixed()) {
            pm->applyForce(force);
        }
    }
}
