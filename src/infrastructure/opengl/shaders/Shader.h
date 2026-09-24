#ifndef BLACK_HOLE_SHADER_H
#define BLACK_HOLE_SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/gl.h>

#include "glm/fwd.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"


class Shader {
public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    void use() const;

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setVec2(const std::string &name, const glm::vec2 &value) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;

    void setMat4(const char * str, const glm::mat4 & mat) const;

private:
    static void checkCompileErrors(unsigned int shader, const std::string& type);
};


#endif
