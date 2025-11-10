#ifndef PBD_X_OPENGLRENDERER_H
#define PBD_X_OPENGLRENDERER_H

#include "Shader.h"
#include <vector>
#include <glm/glm.hpp>

class OpenGLRenderer {
public:
    OpenGLRenderer(int width, int height);
    ~OpenGLRenderer();

    void drawLines(const std::vector<float>& positions, const glm::vec3& color);
    void drawLinesWithColors(const std::vector<float>& positions, const std::vector<glm::vec3>& colors);
    void drawPoints(const std::vector<float>& positions, const glm::vec3& color, float size = 5.0f);

    void setViewportSize(int width, int height);
    void panCamera(float dx, float dy);
    void zoomCamera(float factor);
    void panCameraScreen(double dxPixels, double dyPixels);

private:
    void ensureLineBuffers();
    void ensurePointBuffers();

    int viewportWidth;
    int viewportHeight;

    unsigned int lineVAO{0}, lineVBO{0}, lineColorVBO{0};
    unsigned int pointVAO{0}, pointVBO{0};

    Shader* shader{nullptr};
    // Simple camera state
    float cameraX{0.0f};
    float cameraY{0.0f};
    float cameraZoom{1.0f};
};

#endif //PBD_X_OPENGLRENDERER_H
