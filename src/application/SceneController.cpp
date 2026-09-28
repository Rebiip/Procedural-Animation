#include "SceneController.h"
#include "use_cases/CreatePoint.h"
#include "use_cases/MoveHeadPoint.h"
#include "use_cases/TransformPointsToSnake.h"

SceneController::SceneController(Scene &scene) : scene_(scene) {
}

void SceneController::addPoint(const int x, const int y) {
    scene_.renderables.push_back(CreatePoint::execute(x, y, scene_.jointSize, scene_.jointDistanceConstraintSize));
}

void SceneController::moveHeadPoint(const int x, const int y) {
    MoveHeadPoint::execute(scene_.renderables, x, y);
}

void SceneController::transformPointsToSnake() {
    TransformPointsToSnake::execute(scene_.renderables);
}

void SceneController::increasePointRadius() {
    scene_.jointSize += 5.0f;
}

void SceneController::decreasePointRadius() {
    scene_.jointSize -= 5.0f;
}


void SceneController::increaseJointDistanceConstraint() {
    scene_.jointDistanceConstraintSize += 5.0f;
}

void SceneController::decreaseJointDistanceConstraint() {
    scene_.jointDistanceConstraintSize -= 5.0f;
}

void SceneController::resetApplication() {
    scene_.renderables.clear();
    scene_.jointSize = 10.0f;
    scene_.jointDistanceConstraintSize = 10.0f;
}
