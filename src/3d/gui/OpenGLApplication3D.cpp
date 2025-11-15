#include <glad/glad.h>
#include "OpenGLApplication3D.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
#include <glm/glm.hpp>

OpenGLApplication3D::OpenGLApplication3D(int width, int height) {
    ctx = std::make_unique<GLFWContext>(width, height, "PBD-X 3D Simulation");
    renderer = std::make_unique<OpenGLRenderer3D>(width, height);

    GLFWwindow* window = ctx->getWindow();
    if (window) {
        glfwSetWindowUserPointer(window, this);
        glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) {
            auto app = static_cast<OpenGLApplication3D*>(glfwGetWindowUserPointer(w));
            if (!app) return;
            if (yoffset > 0) {
                app->renderer->zoomCamera(1.0f / app->cameraZoomFactor);
            } else if (yoffset < 0) {
                app->renderer->zoomCamera(app->cameraZoomFactor);
            }
        });
    }

    sim.createCloth(0.0f, 2.0f, 0.0f, 8, 8, 0.2f);
    sim.createRope(3.0f, 0.0f, 0.0f, 10, 0.15f);

    lastTime = glfwGetTime();
}

OpenGLApplication3D::~OpenGLApplication3D() {
    GLFWContext::terminate();
}

void OpenGLApplication3D::processInput() {
    GLFWwindow* window = ctx->getWindow();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    static int spaceState = GLFW_RELEASE;
    int s = glfwGetKey(window, GLFW_KEY_SPACE);
    if (s == GLFW_PRESS && spaceState == GLFW_RELEASE) {
        paused = !paused;
    }
    spaceState = s;

    static int gState = GLFW_RELEASE;
    int g = glfwGetKey(window, GLFW_KEY_G);
    if (g == GLFW_PRESS && gState == GLFW_RELEASE) {
        gravityEnabled = !gravityEnabled;
        std::cout << "Gravity: " << (gravityEnabled ? "ON" : "OFF") << std::endl;
    }
    gState = g;

    static int wState = GLFW_RELEASE;
    int w = glfwGetKey(window, GLFW_KEY_W);
    if (w == GLFW_PRESS && wState == GLFW_RELEASE) {
        windEnabled = !windEnabled;
        std::cout << "Wind: " << (windEnabled ? "ON" : "OFF") << std::endl;
    }
    wState = w;

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    int leftButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (leftButton == GLFW_PRESS) {
        if (!isDraggingCamera) {
            // start drag
            isDraggingCamera = true;
            lastMouseX = mouseX;
            lastMouseY = mouseY;
        } else {
            double dx = mouseX - lastMouseX;
            double dy = mouseY - lastMouseY;
            renderer->rotateCameraY(dx * cameraRotationSpeed * 0.01f);
            renderer->rotateCameraX(-dy * cameraRotationSpeed * 0.01f);
            lastMouseX = mouseX;
            lastMouseY = mouseY;
        }
    } else {
        isDraggingCamera = false;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        renderer->panCamera(0.0f, cameraPanSpeed, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        renderer->panCamera(0.0f, -cameraPanSpeed, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        renderer->panCamera(-cameraPanSpeed, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        renderer->panCamera(cameraPanSpeed, 0.0f, 0.0f);
    }

    static int zState = GLFW_RELEASE;
    int z = glfwGetKey(window, GLFW_KEY_Z);
    if (z == GLFW_PRESS && zState == GLFW_RELEASE) {
        renderer->zoomCamera(1.0f / cameraZoomFactor);
    }
    zState = z;

    static int xState = GLFW_RELEASE;
    int x = glfwGetKey(window, GLFW_KEY_X);
    if (x == GLFW_PRESS && xState == GLFW_RELEASE) {
        renderer->zoomCamera(cameraZoomFactor);
    }
    xState = x;

    static int decState = GLFW_RELEASE;
    int dec = glfwGetKey(window, GLFW_KEY_LEFT_BRACKET);
    if (dec == GLFW_PRESS && decState == GLFW_RELEASE) {
        simulationSpeed = std::max(0.01f, simulationSpeed - 0.05f);
        std::cout << "Simulation speed: " << simulationSpeed << std::endl;
    }
    decState = dec;

    static int incState = GLFW_RELEASE;
    int inc = glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET);
    if (inc == GLFW_PRESS && incState == GLFW_RELEASE) {
        simulationSpeed = std::min(5.0f, simulationSpeed + 0.05f);
        std::cout << "Simulation speed: " << simulationSpeed << std::endl;
    }
    incState = inc;

    static int vState = GLFW_RELEASE;
    int v = glfwGetKey(window, GLFW_KEY_V);
    if (v == GLFW_PRESS && vState == GLFW_RELEASE) {
        showGrid = !showGrid;
        std::cout << "Grid: " << (showGrid ? "ON" : "OFF") << std::endl;
    }
    vState = v;
}

void OpenGLApplication3D::render() {
    int w, h;
    glfwGetFramebufferSize(ctx->getWindow(), &w, &h);
    renderer->setViewportSize(w, h);

    glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (showGrid) {
        renderer->drawGrid(gridSpacing, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    const auto& springs = sim.getSprings();
    std::vector<float> linePositions;
    std::vector<glm::vec3> lineColors;
    linePositions.reserve(springs.size() * 6);
    lineColors.reserve(springs.size() * 2);

    for (Spring* sp : springs) {
        auto p1 = sp->getPointMass1()->getPosition();
        auto p2 = sp->getPointMass2()->getPosition();
        linePositions.push_back(p1.x);
        linePositions.push_back(p1.y);
        linePositions.push_back(p1.z);
        linePositions.push_back(p2.x);
        linePositions.push_back(p2.y);
        linePositions.push_back(p2.z);

        float currentLen = sp->getCurrentLength();
        float restLen = sp->getRestLength();
        float strain = (restLen > 0) ? (currentLen - restLen) / restLen : 0.0f;
        
        strain = std::max(0.0f, std::min(strain, 1.0f));
        glm::vec3 color;
        if (strain < 0.15f) {
            color = glm::vec3(strain / 0.15f, 1.0f, 0.0f);
        } else {
            float t = (strain - 0.15f) / (1.0f - 0.15f);
            color = glm::vec3(1.0f, 1.0f - t, 0.0f);
        }
        
        lineColors.push_back(color);
        lineColors.push_back(color);
    }

    renderer->drawLinesWithColors(linePositions, lineColors);

    const auto& points = sim.getPointMasses();
    std::vector<float> pointPositions;
    pointPositions.reserve(points.size() * 3);
    for (PointMass* pm : points) {
        auto pos = pm->getPosition();
        pointPositions.push_back(pos.x);
        pointPositions.push_back(pos.y);
        pointPositions.push_back(pos.z);
    }

    renderer->drawPoints(pointPositions, {0.2f, 0.7f, 0.9f}, 6.0f);
}

int OpenGLApplication3D::run() {
    GLFWwindow* window = ctx->getWindow();

    while (!ctx->shouldClose()) {
        processInput();
        ctx->pollEvents();

        double now = glfwGetTime();
        double elapsed = now - lastTime;
        lastTime = now;

        if (!paused) {
            float dt = static_cast<float>(elapsed) * simulationSpeed;
            if (dt <= 0.0f) dt = 0.0f;
            if (dt > 0.05f) dt = 0.05f;
            if (gravityEnabled) {
                sim.applyGlobalForce(Vector3D(0, -9.81f, 0));
            }
            if (windEnabled) {
                sim.applyGlobalForce(windForce);
            }
            sim.update(dt);
        }

        render();

        ctx->swapBuffers();
    }

    return 0;
}
