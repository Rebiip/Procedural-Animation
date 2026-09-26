#include "CreatePoint.h"

#include <glm/vec3.hpp>

#include "../../domain/entities/Joint.h"

std::unique_ptr<Renderable> CreatePoint::execute(const int xPos, const int yPos, const float radius) {
    return std::make_unique<Joint>(radius, 24.0f, 45.0f, glm::vec3(0.0f),
                                   glm::vec3(xPos, yPos, 0.0f));
}
