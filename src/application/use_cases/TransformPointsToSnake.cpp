#include "TransformPointsToSnake.h"

#include <algorithm>

#include "../../domain/entities/Joint.h"
#include "../../domain/entities/Snake.h"

void TransformPointsToSnake::execute(std::vector<std::unique_ptr<Renderable> > &renderables) {
    if (renderables.empty()) {
        return;
    }
    if (isAllCircle(renderables)) {
        const Snake snake(toCircle(renderables));
        renderables.clear();
        renderables.push_back(snake.getRenderable());
    }
}

bool TransformPointsToSnake::isAllCircle(const std::vector<std::unique_ptr<Renderable> > &renderables) {
    return std::ranges::all_of(renderables, [](const std::unique_ptr<Renderable> &renderable) {
        return dynamic_cast<Joint *>(renderable.get()) != nullptr;
    });
}

std::vector<Joint> TransformPointsToSnake::toCircle(const std::vector<std::unique_ptr<Renderable> > &vector) {
    std::vector<Joint> circles;
    circles.reserve(vector.size());
    for (const auto &renderable: vector) {
        if (const auto circle = dynamic_cast<Joint *>(renderable.get())) {
            circles.push_back(*circle);
        }
    }
    return circles;
}
