#ifndef PBD_X_OPENGLRENDERER_H
#define PBD_X_OPENGLRENDERER_H

#include "Shader.h"
#include <vector>
#include <glm/glm.hpp>

class OpenGLRenderer3D {
public:
    OpenGLRenderer3D(int width, int height);
    ~OpenGLRenderer3D();

    void drawLines(const std::vector<float>& positions, const glm::vec3& color);
    void drawLinesWithColors(const std::vector<float>& positions, const std::vector<glm::vec3>& colors);
    void drawPoints(const std::vector<float>& positions, const glm::vec3& color, float size = 5.0f);

    // Draw a ground-aligned XZ grid centered at the camera pan position
    void drawGrid(float spacing = 1.0f, const glm::vec3& color = glm::vec3(0.45f, 0.45f, 0.45f));
    // Draw a solid XZ plane under the scene with a single color
    void drawGridSolid(const glm::vec3& color = glm::vec3(0.45f, 0.45f, 0.45f));

    void setViewportSize(int width, int height);
    void rotateCameraX(float angle);
    void rotateCameraY(float angle);
    void panCamera(float dx, float dy, float dz);
    void zoomCamera(float factor);
    // Save the current framebuffer to a PPM image file. Returns true on success.
    bool saveFrameAsPPM(const std::string& path);

private:
    void ensureLineBuffers();
    void ensurePointBuffers();

    int viewportWidth;
    int viewportHeight;

    unsigned int lineVAO{0}, lineVBO{0}, lineColorVBO{0};
    unsigned int pointVAO{0}, pointVBO{0};

    Shader* shader{nullptr};
    // 3D camera state
    float cameraDistance{10.0f};
    float cameraRotX{0.3f};
    float cameraRotY{0.5f};
    float cameraPanX{0.0f};
    float cameraPanY{0.0f};
    float cameraPanZ{0.0f};
};

#endif //PBD_X_OPENGLRENDERER_H
