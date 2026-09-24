#include "CreatePoint.h"

#include <glm/vec3.hpp>

#include "../../domain/entities/Joint.h"

std::unique_ptr<Renderable> CreatePoint::execute(const int xPos, const int yPos) {
    return std::make_unique<Joint>(12.0f, 30.0f, 45.0f, glm::vec3(0.0f),
                                   glm::vec3(xPos, yPos, 0.0f));
}
