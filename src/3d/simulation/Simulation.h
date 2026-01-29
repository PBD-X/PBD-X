#ifndef PBD_X_SIMULATION_H
#define PBD_X_SIMULATION_H

#include <vector>
#include "../core/PointMass.h"
#include "../core/Spring.h"

class Simulation {
public:
    Simulation();
    ~Simulation();

    void update(float dt);
    void setFloorEnabled(bool enabled) { floorEnabled = enabled; }
    void setFloorY(float y) { floorY = y; }
    void setRestitution(float r) { restitution = r; }
    void addPointMass(PointMass* pointMass);
    void addSpring(Spring* spring);

    void createCloth(float startx, float starty, float startz, int width, int height, float spacing);
    void createRope(float startx, float starty, float startz, int numPoints, float spacing);

    [[nodiscard]] const std::vector<PointMass*>& getPointMasses() const { return pointMasses; }
    [[nodiscard]] const std::vector<Spring*>& getSprings() const { return springs; }

    void clear();
    void applyGlobalForce(const Vector3D& force);

private:
    std::vector<PointMass*> pointMasses;
    std::vector<Spring*> springs;
    bool floorEnabled{true};
    float floorY{-1.0f};
    float restitution{0.6f};
};


#endif //PBD_X_SIMULATION_H
