#pragma once
#include "dtos/Scene.h"
#include "ports/ApplicationController.h"

class SceneController final : public ApplicationController {
public:
    explicit SceneController(Scene &scene);

    void addPoint(int x, int y) override;

    void moveHeadPoint(int x, int y) override;

    void transformPointsToSnake() override;

    void increasePointRadius() override;

    void increaseJointDistanceConstraint() override;

    ~SceneController() override = default;

    void decreasePointRadius() override;

    void decreaseJointDistanceConstraint() override;

    void resetApplication() override;

private:
    Scene &scene_;
};
