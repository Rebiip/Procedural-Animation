#include "ProcessInputController.h"

#include <iostream>

#include "./dtos/MouseButton.h"

ProcessInputController::ProcessInputController(ApplicationController &applicationController)
    : applicationController_(applicationController) {
}

void ProcessInputController::onMouseClick(const MouseClickEvent event) const {
    if (event.button == MouseButton::Left) {
        applicationController_.addPoint(static_cast<int>(event.x), static_cast<int>(event.y));
    } else if (event.button == MouseButton::Right) {
        applicationController_.moveHeadPoint(static_cast<int>(event.x), static_cast<int>(event.y));
    }
}

void ProcessInputController::onButtonClick(const int button) const {
    std::cout << "Button clicked: " << button << std::endl;
    if (button == 83) {
        applicationController_.transformPointsToSnake();
    }
}
