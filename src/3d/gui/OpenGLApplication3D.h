#ifndef PBD_X_OPENGLAPPLICATION_H
#define PBD_X_OPENGLAPPLICATION_H

#include "GLFWContext.h"
#include "OpenGLRenderer3D.h"
#include "../simulation/Simulation.h"
#include "../core/Vector3D.h"
#include <memory>

class OpenGLApplication3D {
public:
    OpenGLApplication3D(int width = 1024, int height = 768);
    ~OpenGLApplication3D();

    int run();

private:
    void processInput();
    void render();

    std::unique_ptr<GLFWContext> ctx;
    std::unique_ptr<OpenGLRenderer3D> renderer;
    Simulation sim;

    bool paused{false};
    double lastTime{0.0};
    bool gravityEnabled{true};
    bool windEnabled{false};
    Vector3D windForce{5.0f, 0.0f, 5.0f};
    float cameraRotationSpeed{0.05f};
    float cameraZoomFactor{1.1f};
    // Smaller pan step for 3D camera (configurable)
    float cameraPanSpeed{0.02f};
    // Grid toggle
    bool showGrid{false};
    float gridSpacing{0.5f};
    bool isDraggingCamera{false};
    double lastMouseX{0.0}, lastMouseY{0.0};
    float simulationSpeed{0.5f};
};

#endif //PBD_X_OPENGLAPPLICATION_H
