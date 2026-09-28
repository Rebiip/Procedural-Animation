#include "SceneController.h"
#include "use_cases/CreatePoint.h"
#include "use_cases/MoveHeadPoint.h"
#include "use_cases/TransformPointsToSnake.h"

SceneController::SceneController(Scene &scene) : scene_(scene) {
}

void SceneController::addPoint(const int x, const int y) {
    scene_.renderables.push_back(CreatePoint::execute(x, y, pointRadius_, jointDistanceConstraint_));
}

void SceneController::moveHeadPoint(const int x, const int y) {
    MoveHeadPoint::execute(scene_.renderables, x, y);
}

void SceneController::transformPointsToSnake() {
    TransformPointsToSnake::execute(scene_.renderables);
}

void SceneController::increasePointRadius() {
    pointRadius_ += 5.0f;
}

void SceneController::decreasePointRadius() {
    pointRadius_ -= 5.0f;
}


void SceneController::increaseJointDistanceConstraint() {
    jointDistanceConstraint_ += 5.0f;
}

void SceneController::decreaseJointDistanceConstraint() {
    jointDistanceConstraint_ -= 5.0f;
}
