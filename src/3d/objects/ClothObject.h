#ifndef PBD_X_CLOTHOBJECT_H
#define PBD_X_CLOTHOBJECT_H

#include <vector>
#include "../core/PointMass.h"
#include "../core/Spring.h"

class ClothObject {
public:
    ClothObject(float startX, float startY, float startZ, int width, int height, float spacing);
    ~ClothObject();

    [[nodiscard]] const std::vector<PointMass*>& getPointMasses() const { return pointMasses; }
    [[nodiscard]] const std::vector<Spring*>& getSprings() const { return springs; }

    void setCornersFixed(bool fixed);
    void applyWindForce(const Vector3D& force);

private:
    std::vector<std::vector<PointMass*>> grid;
    std::vector<PointMass*> pointMasses;
    std::vector<Spring*> springs;
    int width, height;

    void createSprings();
};


#endif //PBD_X_CLOTHOBJECT_H
