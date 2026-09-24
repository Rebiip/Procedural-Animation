#include "DistanceConstraintPolicy.h"

#include <glm/geometric.hpp>

glm::vec3 DistanceConstraintPolicy::apply(const Joint &jointTo, const Joint &jointFrom) {
    const auto offset = jointTo.getTranslation() - jointFrom.getTranslation();
    const auto distance = glm::length(offset);
    const auto direction = distance > 0.0f ? offset / distance : glm::vec3(-1.0f, 0.0f, 0.0f);
    return jointFrom.getTranslation() + direction * jointFrom.getAnchorRadius();
}
