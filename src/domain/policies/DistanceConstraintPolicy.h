#pragma once
#include "../../domain/entities/Joint.h"

class DistanceConstraintPolicy {
public:
    DistanceConstraintPolicy() = delete;

    explicit DistanceConstraintPolicy(const float speed) : speed(speed) {
    }

    ~DistanceConstraintPolicy() = default;

    static glm::vec3 apply(const Joint &jointTo, const Joint &jointFrom);

private:
    const float speed;
};
