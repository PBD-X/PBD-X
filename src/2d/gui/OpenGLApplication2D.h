#ifndef PBD_X_OPENGLAPPLICATION_H
#define PBD_X_OPENGLAPPLICATION_H

#include "GLFWContext.h"
#include "OpenGLRenderer2D.h"
#include "../simulation/Simulation.h"
#include "../core/Vector2D.h"
#include <memory>

class OpenGLApplication2D {
public:
    OpenGLApplication2D(int width = 1024, int height = 768);
    ~OpenGLApplication2D();

    int run();

private:
    void processInput();
    void render();

    std::unique_ptr<GLFWContext> ctx;
    std::unique_ptr<OpenGLRenderer2D> renderer;
    Simulation sim;

    bool paused{false};
    double lastTime{0.0};
    bool gravityEnabled{true};
    bool windEnabled{false};
    Vector2D windForce{5.0f, 0.0f};
    float cameraPanSpeed{0.5f};
    float cameraZoomFactor{1.1f};
    bool isDraggingCamera{false};
    double lastMouseX{0.0}, lastMouseY{0.0};
    float simulationSpeed{0.5f};
    // Grid toggle
    bool showGrid{false};
    float gridSpacing{0.5f};
};

#endif //PBD_X_OPENGLAPPLICATION_H
