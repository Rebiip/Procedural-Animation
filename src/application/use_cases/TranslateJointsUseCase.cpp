#include "TranslateJointsUseCase.h"

#include <algorithm>
#include <cmath>
#include <glm/geometric.hpp>

#include "../../domain/policies/AngleConstraintPolicy.h"
#include "../../domain/policies/DistanceConstraintPolicy.h"

void TranslateJointsUseCase::execute(std::vector<Joint> &joints, const glm::vec3 *destination, const float deltaTime) {
    std::vector<Joint *> jointPointers;
    jointPointers.reserve(joints.size());
    for (auto &joint: joints) {
        jointPointers.push_back(&joint);
    }
    execute(jointPointers, destination, deltaTime);
}


void TranslateJointsUseCase::execute(const std::vector<Joint *> &joints, const glm::vec3 *destination,
                                     const float deltaTime) {
    if (joints.empty() || destination == nullptr || !std::isfinite(deltaTime) || deltaTime <= 0.0f) {
        return;
    }

    auto position = joints.front()->getTranslation();
    constexpr float speed = 30.0f;

    auto movement = get_movement(position, *destination, speed, deltaTime);
    joints.front()->translateTo(movement.x, movement.y);


    for (std::size_t i = 1; i < joints.size(); ++i) {
        position = joints[i]->getTranslation();
        auto target = DistanceConstraintPolicy::apply(*joints[i], *joints[i - 1]);

        if (i >= 2) {
            const auto incomingDirection = joints[i - 1]->getTranslation() - joints[i - 2]->getTranslation();
            target = AngleConstraintPolicy(joints[i]->getRestrictionAngle()).apply(
                incomingDirection, *joints[i - 1], target);
        }
        movement = get_movement(position, target, speed, deltaTime);
        joints[i]->translateTo(movement.x, movement.y);
    }
}

glm::vec3 TranslateJointsUseCase::get_movement(const glm::vec3 position, const glm::vec3 destination, const float speed,
                                               const float deltaTime) {
    const auto offset = glm::vec3(destination.x, destination.y, 0.0f) - position;
    glm::vec3 movement = destination;

    if (const auto distance = glm::length(offset); distance > 0.0f) {
        movement = position + offset * (std::min(speed * deltaTime, distance) / distance);
    }

    return movement;
}
