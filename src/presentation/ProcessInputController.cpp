#include "ProcessInputController.h"

#include <ostream>

#include "./dtos/MouseButton.h"

#include "../application/use_cases/CreatePoint.h"
#include "../application/use_cases/MoveHeadPoint.h"
#include "../application/use_cases/TransformPointsToSnake.h"

ProcessInputController::ProcessInputController(Scene &scene) : scene_(scene) {
}

void ProcessInputController::onMouseClick(const MouseClickEvent event) const {
    if (event.button == MouseButton::Left) {
        scene_.renderables.push_back(CreatePoint::execute(static_cast<int>(event.x), static_cast<int>(event.y)));
    } else if (event.button == MouseButton::Right) {
        MoveHeadPoint::execute(scene_.renderables, static_cast<int>(event.x), static_cast<int>(event.y));
    }
}

void ProcessInputController::onButtonClick(const int button) const {
    if (button == 83) {
        TransformPointsToSnake::execute(scene_.renderables);
    }
}
