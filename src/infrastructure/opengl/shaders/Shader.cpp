#include "Shader.h"
#include <stdexcept>


static std::string readFileToString(const char *path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error(std::string("Could not open shader: ") + path);
    }
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

Shader::Shader(const char *vertexPath, const char *fragmentPath) : ID(0) {
    const std::string vertexCode = readFileToString(vertexPath);
    const std::string fragmentCode = readFileToString(fragmentPath);
    const char *vertexSource = vertexCode.c_str();
    const char *fragmentSource = fragmentCode.c_str();
    const GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    const GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    try {
        glShaderSource(vertex, 1, &vertexSource, nullptr);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        glShaderSource(fragment, 1, &fragmentSource, nullptr);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
    } catch (...) {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (ID) glDeleteProgram(ID);
        throw;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    if (ID) glDeleteProgram(ID);
}

void Shader::use() const {
    if (ID != 0) {
        glUseProgram(ID);
    }
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat4(const char *str, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, str), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::checkCompileErrors(const unsigned int shader, const std::string &type) {
    GLint success = GL_FALSE;
    char infoLog[4096]{};
    if (type == "PROGRAM") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) glGetProgramInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
    }
    if (!success) {
        throw std::runtime_error("Shader " + type + " failed: " + infoLog);
    }
}
