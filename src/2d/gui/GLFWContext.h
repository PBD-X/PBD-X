#ifndef PBD_X_GLFWCONTEXT_H
#define PBD_X_GLFWCONTEXT_H

#include <string>
#include <GLFW/glfw3.h>

class GLFWContext {
public:
    GLFWContext(int width, int height, const std::string& title);
    ~GLFWContext();

    bool shouldClose() const;
    void pollEvents() const;
    void swapBuffers() const;
    GLFWwindow* getWindow() const { return window; }

    static void initialize();
    static void terminate();

private:
    GLFWwindow* window{nullptr};
};

#endif //PBD_X_GLFWCONTEXT_H
