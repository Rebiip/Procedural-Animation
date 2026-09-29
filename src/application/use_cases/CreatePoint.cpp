#include "CreatePoint.h"

#include <glm/vec3.hpp>

#include "../../domain/entities/Joint.h"

std::unique_ptr<Renderable> CreatePoint::execute(const int xPos, const int yPos, const float radius,
                                                 const float jointAnchorRadius, const float maxAngleConstraint) {
    return std::make_unique<Joint>(radius, jointAnchorRadius, maxAngleConstraint, glm::vec3(0.0f),
                                   glm::vec3(xPos, yPos, 0.0f));
}
