#include <glad/glad.h>
#include "OpenGLRenderer3D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

static const char* vertexSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
uniform mat4 uProjection;
uniform mat4 uView;
out vec3 vertexColor;
void main() {
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
    vertexColor = aColor;
}
)";

static const char* fragmentSrc = R"(
#version 330 core
in vec3 vertexColor;
uniform vec3 uColor;
out vec4 FragColor;
void main() {
    // If vertex color is provided (non-zero), use it; otherwise use uniform color
    if (length(vertexColor) > 0.0) {
        FragColor = vec4(vertexColor, 1.0);
    } else {
        FragColor = vec4(uColor, 1.0);
    }
}
)";

bool OpenGLRenderer3D::saveFrameAsPPM(const std::string& path) {
    if (viewportWidth <= 0 || viewportHeight <= 0) return false;
    int w = viewportWidth;
    int h = viewportHeight;
    std::vector<unsigned char> pixels(w * h * 4);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    std::ofstream out(path, std::ios::binary);
    if (!out) return false;
    out << "P6\n" << w << " " << h << "\n255\n";
    for (int y = h - 1; y >= 0; --y) {
        for (int x = 0; x < w; ++x) {
            size_t idx = (y * w + x) * 4;
            unsigned char r = pixels[idx + 0];
            unsigned char g = pixels[idx + 1];
            unsigned char b = pixels[idx + 2];
            out.put((char)r);
            out.put((char)g);
            out.put((char)b);
        }
    }
    out.close();
    return true;
}

OpenGLRenderer3D::OpenGLRenderer3D(int width, int height)
    : viewportWidth(width), viewportHeight(height) {
    shader = new Shader(vertexSrc, fragmentSrc);
    ensureLineBuffers();
    ensurePointBuffers();
}

void OpenGLRenderer3D::rotateCameraX(float angle) {
    cameraRotX += angle;
}

void OpenGLRenderer3D::rotateCameraY(float angle) {
    cameraRotY += angle;
}

void OpenGLRenderer3D::panCamera(float dx, float dy, float dz) {
    cameraPanX += dx;
    cameraPanY += dy;
    cameraPanZ += dz;
}

void OpenGLRenderer3D::zoomCamera(float factor) {
    cameraDistance *= factor;
    if (cameraDistance < 1.0f) cameraDistance = 1.0f;
    if (cameraDistance > 50.0f) cameraDistance = 50.0f;
}

OpenGLRenderer3D::~OpenGLRenderer3D() {
    delete shader;
    if (lineVBO) glDeleteBuffers(1, &lineVBO);
    if (lineColorVBO) glDeleteBuffers(1, &lineColorVBO);
    if (lineVAO) glDeleteVertexArrays(1, &lineVAO);
    if (pointVBO) glDeleteBuffers(1, &pointVBO);
    if (pointVAO) glDeleteVertexArrays(1, &pointVAO);
}

void OpenGLRenderer3D::ensureLineBuffers() {
    if (lineVAO == 0) {
        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineVBO);
        glGenBuffers(1, &lineColorVBO);
        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, lineColorVBO);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindVertexArray(0);
    }
}

void OpenGLRenderer3D::ensurePointBuffers() {
    if (pointVAO == 0) {
        glGenVertexArrays(1, &pointVAO);
        glGenBuffers(1, &pointVBO);
        glBindVertexArray(pointVAO);
        glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindVertexArray(0);
    }
}

void OpenGLRenderer3D::setViewportSize(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
    glViewport(0, 0, width, height);
}

void OpenGLRenderer3D::drawLines(const std::vector<float>& positions, const glm::vec3& color) {
    if (positions.empty()) return;
    shader->bind();

    float aspect = (float)viewportWidth / (float)viewportHeight;
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    
    // Build view matrix with current camera rotation and distance
    glm::vec3 camPos(
        cameraPanX + cameraDistance * sin(cameraRotY) * cos(cameraRotX),
        cameraPanY + cameraDistance * sin(cameraRotX),
        cameraPanZ + cameraDistance * cos(cameraRotY) * cos(cameraRotX)
    );
    glm::mat4 view = glm::lookAt(camPos, glm::vec3(cameraPanX, cameraPanY, cameraPanZ), glm::vec3(0.0f, 1.0f, 0.0f));
    
    shader->setUniformMat4("uProjection", glm::value_ptr(proj));
    shader->setUniformMat4("uView", glm::value_ptr(view));
    
    int loc = glGetUniformLocation(shader->getProgram(), "uColor");
    glUniform3f(loc, color.r, color.g, color.b);

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, lineColorVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, (GLsizei)(positions.size() / 3));
    glBindVertexArray(0);

    shader->unbind();
}

void OpenGLRenderer3D::drawLinesWithColors(const std::vector<float>& positions, const std::vector<glm::vec3>& colors) {
    if (positions.empty() || colors.empty()) return;
    shader->bind();

    float aspect = (float)viewportWidth / (float)viewportHeight;
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    
    // Build view matrix with current camera rotation and distance
    glm::vec3 camPos(
        cameraPanX + cameraDistance * sin(cameraRotY) * cos(cameraRotX),
        cameraPanY + cameraDistance * sin(cameraRotX),
        cameraPanZ + cameraDistance * cos(cameraRotY) * cos(cameraRotX)
    );
    glm::mat4 view = glm::lookAt(camPos, glm::vec3(cameraPanX, cameraPanY, cameraPanZ), glm::vec3(0.0f, 1.0f, 0.0f));
    
    shader->setUniformMat4("uProjection", glm::value_ptr(proj));
    shader->setUniformMat4("uView", glm::value_ptr(view));

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, lineColorVBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, (GLsizei)(positions.size() / 3));
    glBindVertexArray(0);

    shader->unbind();
}

void OpenGLRenderer3D::drawPoints(const std::vector<float>& positions, const glm::vec3& color, float size) {
    if (positions.empty()) return;
    shader->bind();

    float aspect = (float)viewportWidth / (float)viewportHeight;
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    
    // Build view matrix with current camera rotation and distance
    glm::vec3 camPos(
        cameraPanX + cameraDistance * sin(cameraRotY) * cos(cameraRotX),
        cameraPanY + cameraDistance * sin(cameraRotX),
        cameraPanZ + cameraDistance * cos(cameraRotY) * cos(cameraRotX)
    );
    glm::mat4 view = glm::lookAt(camPos, glm::vec3(cameraPanX, cameraPanY, cameraPanZ), glm::vec3(0.0f, 1.0f, 0.0f));
    
    shader->setUniformMat4("uProjection", glm::value_ptr(proj));
    shader->setUniformMat4("uView", glm::value_ptr(view));
    
    int loc = glGetUniformLocation(shader->getProgram(), "uColor");
    glUniform3f(loc, color.r, color.g, color.b);

    glPointSize(size);
    glBindVertexArray(pointVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_POINTS, 0, (GLsizei)(positions.size() / 3));
    glBindVertexArray(0);

    shader->unbind();
}

void OpenGLRenderer3D::drawGrid(float spacing, const glm::vec3& color) {
    if (spacing <= 0.0f) spacing = 1.0f;

    // Determine extent based on camera distance
    float extent = cameraDistance * 1.5f;
    int lines = (int)std::ceil((extent * 2.0f) / spacing);
    if (lines < 2) lines = 2;

    float half = extent;
    std::vector<float> positions;
    // lines parallel to Z (vary X)
    for (float x = -half; x <= half; x += spacing) {
        positions.push_back(x + cameraPanX);
        positions.push_back(cameraPanY);
        positions.push_back(-half + cameraPanZ);

        positions.push_back(x + cameraPanX);
        positions.push_back(cameraPanY);
        positions.push_back(half + cameraPanZ);
    }
    // lines parallel to X (vary Z)
    for (float z = -half; z <= half; z += spacing) {
        positions.push_back(-half + cameraPanX);
        positions.push_back(cameraPanY);
        positions.push_back(z + cameraPanZ);

        positions.push_back(half + cameraPanX);
        positions.push_back(cameraPanY);
        positions.push_back(z + cameraPanZ);
    }

    if (!positions.empty()) {
        // draw as lines using uniform color (disable per-vertex color attribute)
        shader->bind();
        float aspect = (float)viewportWidth / (float)viewportHeight;
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        glm::vec3 camPos(
            cameraPanX + cameraDistance * sin(cameraRotY) * cos(cameraRotX),
            cameraPanY + cameraDistance * sin(cameraRotX),
            cameraPanZ + cameraDistance * cos(cameraRotY) * cos(cameraRotX)
        );
        glm::mat4 view = glm::lookAt(camPos, glm::vec3(cameraPanX, cameraPanY, cameraPanZ), glm::vec3(0.0f, 1.0f, 0.0f));
        shader->setUniformMat4("uProjection", glm::value_ptr(proj));
        shader->setUniformMat4("uView", glm::value_ptr(view));
        int loc = glGetUniformLocation(shader->getProgram(), "uColor");
        glUniform3f(loc, color.r, color.g, color.b);

        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);
        glDisableVertexAttribArray(1);
        glDrawArrays(GL_LINES, 0, (GLsizei)(positions.size() / 3));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

        shader->unbind();
    }
}

void OpenGLRenderer3D::drawGridSolid(const glm::vec3& color) {
    // Determine extent based on camera distance
    float extent = cameraDistance * 1.5f;
    float half = extent;

    // Four corners (XZ plane) at cameraPanY height
    float lx = -half + cameraPanX;
    float rx = half + cameraPanX;
    float lz = -half + cameraPanZ;
    float rz = half + cameraPanZ;
    float y = cameraPanY;

    std::vector<float> verts = {
        lx, y, lz,
        rx, y, lz,
        rx, y, rz,

        lx, y, lz,
        rx, y, rz,
        lx, y, rz
    };

    shader->bind();
    float aspect = (float)viewportWidth / (float)viewportHeight;
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    glm::vec3 camPos(
        cameraPanX + cameraDistance * sin(cameraRotY) * cos(cameraRotX),
        cameraPanY + cameraDistance * sin(cameraRotX),
        cameraPanZ + cameraDistance * cos(cameraRotY) * cos(cameraRotX)
    );
    glm::mat4 view = glm::lookAt(camPos, glm::vec3(cameraPanX, cameraPanY, cameraPanZ), glm::vec3(0.0f, 1.0f, 0.0f));

    shader->setUniformMat4("uProjection", glm::value_ptr(proj));
    shader->setUniformMat4("uView", glm::value_ptr(view));
    int loc = glGetUniformLocation(shader->getProgram(), "uColor");
    glUniform3f(loc, color.r, color.g, color.b);

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    shader->unbind();
}
