#include "UpdateScene.h"
#include "TranslateJointsUseCase.h"

#include "../../domain/entities/Joint.h"
#include "../../domain/entities/Snake.h"

void UpdateScene::execute(const Scene &scene, const float deltaTime) {
    std::vector<Joint *> points;
    std::vector<Snake *> snakes;
    for (const auto &renderable: scene.renderables) {
        if (const auto circle = dynamic_cast<Joint *>(renderable.get())) {
            points.push_back(circle);
        }
        if (const auto snake = dynamic_cast<Snake *>(renderable.get())) {
            snakes.push_back(snake);
        }
    }
    if (!points.empty()) {
        const auto destination = points.front()->getDestination();
        TranslateJointsUseCase::execute(points, &destination, deltaTime);
    }
    for (const auto &snake: snakes) {
        snake->update(deltaTime);
    }
}
