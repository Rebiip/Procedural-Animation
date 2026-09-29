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
    } else if (button == 61) {
        applicationController_.increasePointRadius();
    } else if (button == 45) {
        applicationController_.decreasePointRadius();
    } else if (button == 46) {
        applicationController_.increaseJointDistanceConstraint();
    } else if (button == 44) {
        applicationController_.decreaseJointDistanceConstraint();
    } else if (button == 82) {
        applicationController_.resetApplication();
    } else if (button == 81) {
        applicationController_.increaseMaxAngleConstraint();
    } else if (button == 87) {
        applicationController_.decreaseMaxAngleConstraint();
    }
}
