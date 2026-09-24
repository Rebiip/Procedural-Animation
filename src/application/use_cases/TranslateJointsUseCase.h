#pragma once
#include <vector>

#include "../../domain/entities/Joint.h"

class TranslateJointsUseCase {
public:
    TranslateJointsUseCase() = default;

    ~TranslateJointsUseCase() = default;

    static void execute(const std::vector<Joint *> &joints, const glm::vec3 *destination, float deltaTime);

    static void execute(std::vector<Joint> &joints, const glm::vec3 *destination, float deltaTime);

private:
    static glm::vec3 get_movement(const glm::vec3 position, const glm::vec3 destination, const float speed, const float deltaTime);
};
