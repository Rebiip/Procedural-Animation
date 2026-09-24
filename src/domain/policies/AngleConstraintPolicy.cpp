#include "AngleConstraintPolicy.h"

#include <cmath>
#include <glm/glm.hpp>

glm::vec3 AngleConstraintPolicy::apply(const glm::vec3 previousSegmentDirection, const Joint &pivot,
                                       const glm::vec3 desiredPosition) const {
    const auto offset = desiredPosition - pivot.getTranslation();
    const auto length = glm::length(offset);
    if (length == 0.0f || glm::length(previousSegmentDirection) == 0.0f) {
        return desiredPosition;
    }
    const auto direction = glm::normalize(previousSegmentDirection);
    const auto angle = std::atan2(direction.x * offset.y - direction.y * offset.x,
                                  glm::dot(direction, offset));
    const auto limit = glm::radians(glm::clamp(maxAngle_, 0.0f, 180.0f));
    const auto clampedAngle = glm::clamp(angle, -limit, limit);
    const auto cosine = std::cos(clampedAngle);
    const auto sine = std::sin(clampedAngle);
    return pivot.getTranslation() + length * glm::vec3(
               direction.x * cosine - direction.y * sine,
               direction.x * sine + direction.y * cosine, 0.0f);
}
