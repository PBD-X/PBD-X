#include <glad/glad.h>
#include "Shader.h"
#include <stdexcept>
#include <iostream>

unsigned int Shader::compile(unsigned int type, const std::string& src) {
    unsigned int id = glCreateShader(type);
    const char* cstr = src.c_str();
    glShaderSource(id, 1, &cstr, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::string message(length, '\0');
        glGetShaderInfoLog(id, length, &length, &message[0]);
        std::cerr << "Shader compile error: " << message << std::endl;
        glDeleteShader(id);
        throw std::runtime_error("Shader compilation failed");
    }
    return id;
}

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {
    unsigned int vs = compile(GL_VERTEX_SHADER, vertexSrc);
    unsigned int fs = compile(GL_FRAGMENT_SHADER, fragmentSrc);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::string message(length, '\0');
        glGetProgramInfoLog(program, length, &length, &message[0]);
        glDeleteProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);
        std::cerr << "Shader link error: " << message << std::endl;
        throw std::runtime_error("Shader linking failed");
    }

    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader() {
    if (program) glDeleteProgram(program);
}

void Shader::bind() const {
    glUseProgram(program);
}

void Shader::unbind() const {
    glUseProgram(0);
}

void Shader::setUniformMat4(const std::string& name, const float* matrix) const {
    int loc = glGetUniformLocation(program, name.c_str());
    if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, matrix);
}
