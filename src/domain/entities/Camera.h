#pragma once
#include <glm/glm.hpp>

struct Camera {
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec3 forward{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};

    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    bool orthographic = true;
};
