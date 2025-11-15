#include <glad/glad.h>
#include "GLFWContext.h"
#include <stdexcept>
#include <iostream>

static bool glfwInitialized = false;

void GLFWContext::initialize() {
    if (!glfwInitialized) {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        glfwInitialized = true;
    }
}

void GLFWContext::terminate() {
    if (glfwInitialized) {
        glfwTerminate();
        glfwInitialized = false;
    }
}

GLFWContext::GLFWContext(int width, int height, const std::string& title) {
    initialize();

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        terminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        terminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

GLFWContext::~GLFWContext() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
}

bool GLFWContext::shouldClose() const {
    return window ? glfwWindowShouldClose(window) : true;
}

void GLFWContext::pollEvents() const {
    glfwPollEvents();
}

void GLFWContext::swapBuffers() const {
    if (window) glfwSwapBuffers(window);
}
