#ifndef PBD_X_SHADER_H
#define PBD_X_SHADER_H

#include <string>
#include <GLFW/glfw3.h>

class Shader {
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void bind() const;
    void unbind() const;

    unsigned int getProgram() const { return program; }
    void setUniformMat4(const std::string& name, const float* matrix) const;

private:
    unsigned int program{0};
    unsigned int compile(unsigned int type, const std::string& src);
};

#endif //PBD_X_SHADER_H
