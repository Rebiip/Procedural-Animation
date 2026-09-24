#pragma once
#include <vector>
#include "../../domain/entities/Joint.h"

class AngleConstraintPolicy {
public:
    explicit AngleConstraintPolicy(const float maxAngle) : maxAngle_(maxAngle) {
    }


    ~AngleConstraintPolicy() = default;

    [[nodiscard]] glm::vec3 apply(glm::vec3 previousSegmentDirection, const Joint &pivot, glm::vec3 desiredPosition) const;

private:
    const float maxAngle_;
};
