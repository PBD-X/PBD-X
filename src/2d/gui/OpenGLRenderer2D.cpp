#include <glad/glad.h>
#include "OpenGLRenderer2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

static const char* vertexSrc = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
uniform mat4 uProjection;
out vec3 vertexColor;
void main() {
    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
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

OpenGLRenderer2D::OpenGLRenderer2D(int width, int height)
    : viewportWidth(width), viewportHeight(height) {
    shader = new Shader(vertexSrc, fragmentSrc);
    ensureLineBuffers();
    ensurePointBuffers();
}

void OpenGLRenderer2D::panCamera(float dx, float dy) {
    cameraX += dx;
    cameraY += dy;
}

void OpenGLRenderer2D::zoomCamera(float factor) {
    cameraZoom *= factor;
    if (cameraZoom < 0.05f) cameraZoom = 0.05f;
    if (cameraZoom > 20.0f) cameraZoom = 20.0f;
}

void OpenGLRenderer2D::panCameraScreen(double dxPixels, double dyPixels) {
    if (viewportWidth == 0 || viewportHeight == 0) return;
    float aspect = (float)viewportWidth / (float)viewportHeight;
    float worldWidth = 10.0f * aspect;
    float worldHeight = 10.0f;
    worldWidth /= cameraZoom;
    worldHeight /= cameraZoom;

    float worldDX = -((float)dxPixels / (float)viewportWidth) * worldWidth; // drag right -> move camera right
    float worldDY = ((float)dyPixels / (float)viewportHeight) * worldHeight; // drag down -> move camera down

    panCamera(worldDX, worldDY);
}

OpenGLRenderer2D::~OpenGLRenderer2D() {
    delete shader;
    if (lineVBO) glDeleteBuffers(1, &lineVBO);
    if (lineColorVBO) glDeleteBuffers(1, &lineColorVBO);
    if (lineVAO) glDeleteVertexArrays(1, &lineVAO);
    if (pointVBO) glDeleteBuffers(1, &pointVBO);
    if (pointVAO) glDeleteVertexArrays(1, &pointVAO);
}

void OpenGLRenderer2D::ensureLineBuffers() {
    if (lineVAO == 0) {
        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineVBO);
        glGenBuffers(1, &lineColorVBO);
        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, lineColorVBO);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindVertexArray(0);
    }
}

void OpenGLRenderer2D::ensurePointBuffers() {
    if (pointVAO == 0) {
        glGenVertexArrays(1, &pointVAO);
        glGenBuffers(1, &pointVBO);
        glBindVertexArray(pointVAO);
        glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glBindVertexArray(0);
    }
}

void OpenGLRenderer2D::setViewportSize(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
    glViewport(0, 0, width, height);
}

void OpenGLRenderer2D::drawLines(const std::vector<float>& positions, const glm::vec3& color) {
    if (positions.empty()) return;
    shader->bind();

    float aspect = (float)viewportWidth / (float)viewportHeight;
    float worldWidth = 10.0f * aspect;
    float worldHeight = 10.0f;
    worldWidth /= cameraZoom;
    worldHeight /= cameraZoom;
    float left = cameraX - worldWidth/2.0f;
    float right = cameraX + worldWidth/2.0f;
    float bottom = cameraY - worldHeight/2.0f;
    float top = cameraY + worldHeight/2.0f;
    glm::mat4 proj = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    shader->setUniformMat4("uProjection", glm::value_ptr(proj));
    int loc = glGetUniformLocation(shader->getProgram(), "uColor");
    glUniform3f(loc, color.r, color.g, color.b);

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);
    glDisableVertexAttribArray(1);
    glDrawArrays(GL_LINES, 0, (GLsizei)(positions.size() / 2));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    shader->unbind();
}

void OpenGLRenderer2D::drawLinesWithColors(const std::vector<float>& positions, const std::vector<glm::vec3>& colors) {
    if (positions.empty() || colors.empty()) return;
    shader->bind();

    float aspect = (float)viewportWidth / (float)viewportHeight;
    float worldWidth = 10.0f * aspect;
    float worldHeight = 10.0f;
    worldWidth /= cameraZoom;
    worldHeight /= cameraZoom;
    float left = cameraX - worldWidth/2.0f;
    float right = cameraX + worldWidth/2.0f;
    float bottom = cameraY - worldHeight/2.0f;
    float top = cameraY + worldHeight/2.0f;
    glm::mat4 proj = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    shader->setUniformMat4("uProjection", glm::value_ptr(proj));

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, lineColorVBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, (GLsizei)(positions.size() / 2));
    glBindVertexArray(0);

    shader->unbind();
}

void OpenGLRenderer2D::drawPoints(const std::vector<float>& positions, const glm::vec3& color, float size) {
    if (positions.empty()) return;
    shader->bind();

    float aspect = (float)viewportWidth / (float)viewportHeight;
    float worldWidth = 10.0f * aspect;
    float worldHeight = 10.0f;
    worldWidth /= cameraZoom;
    worldHeight /= cameraZoom;
    float left = cameraX - worldWidth/2.0f;
    float right = cameraX + worldWidth/2.0f;
    float bottom = cameraY - worldHeight/2.0f;
    float top = cameraY + worldHeight/2.0f;
    glm::mat4 proj = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    shader->setUniformMat4("uProjection", glm::value_ptr(proj));
    int loc = glGetUniformLocation(shader->getProgram(), "uColor");
    glUniform3f(loc, color.r, color.g, color.b);

    glPointSize(size);
    glBindVertexArray(pointVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_POINTS, 0, (GLsizei)(positions.size() / 2));
    glBindVertexArray(0);

    shader->unbind();
}

void OpenGLRenderer2D::drawGrid(float spacing, const glm::vec3& color) {
    float aspect = (float)viewportWidth / (float)viewportHeight;
    float worldWidth = 10.0f * aspect;
    float worldHeight = 10.0f;
    worldWidth /= cameraZoom;
    worldHeight /= cameraZoom;
    float left = cameraX - worldWidth/2.0f;
    float right = cameraX + worldWidth/2.0f;
    float bottom = cameraY - worldHeight/2.0f;
    float top = cameraY + worldHeight/2.0f;

    if (spacing <= 0.0f) spacing = 1.0f;

    float startX = std::floor(left / spacing) * spacing;
    float endX = std::ceil(right / spacing) * spacing;
    float startY = std::floor(bottom / spacing) * spacing;
    float endY = std::ceil(top / spacing) * spacing;

    std::vector<float> positions;
    for (float x = startX; x <= endX; x += spacing) {
        positions.push_back(x);
        positions.push_back(startY);
        positions.push_back(x);
        positions.push_back(endY);
    }
    for (float y = startY; y <= endY; y += spacing) {
        positions.push_back(startX);
        positions.push_back(y);
        positions.push_back(endX);
        positions.push_back(y);
    }

    if (!positions.empty()) {
        shader->bind();
        float aspect = (float)viewportWidth / (float)viewportHeight;
        float worldWidth = 10.0f * aspect;
        float worldHeight = 10.0f;
        worldWidth /= cameraZoom;
        worldHeight /= cameraZoom;
        float left = cameraX - worldWidth/2.0f;
        float right = cameraX + worldWidth/2.0f;
        float bottom = cameraY - worldHeight/2.0f;
        float top = cameraY + worldHeight/2.0f;
        glm::mat4 proj = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        shader->setUniformMat4("uProjection", glm::value_ptr(proj));
        int loc = glGetUniformLocation(shader->getProgram(), "uColor");
        glUniform3f(loc, color.r, color.g, color.b);

        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);
        glDisableVertexAttribArray(1);
        glDrawArrays(GL_LINES, 0, (GLsizei)(positions.size() / 2));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

        shader->unbind();
    }
}

void OpenGLRenderer2D::drawGridSolid(const glm::vec3& color) {
    float aspect = (float)viewportWidth / (float)viewportHeight;
    float worldWidth = 10.0f * aspect;
    float worldHeight = 10.0f;
    worldWidth /= cameraZoom;
    worldHeight /= cameraZoom;
    float left = cameraX - worldWidth/2.0f;
    float right = cameraX + worldWidth/2.0f;
    float bottom = cameraY - worldHeight/2.0f;
    float top = cameraY + worldHeight/2.0f;

    std::vector<float> verts = {
        left, bottom,
        right, bottom,
        right, top,

        left, bottom,
        right, top,
        left, top
    };

    shader->bind();
    glm::mat4 proj = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    shader->setUniformMat4("uProjection", glm::value_ptr(proj));
    int loc = glGetUniformLocation(shader->getProgram(), "uColor");
    glUniform3f(loc, color.r, color.g, color.b);

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    shader->unbind();
}
