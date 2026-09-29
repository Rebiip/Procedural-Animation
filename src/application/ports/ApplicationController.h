#pragma once

class ApplicationController {
public:
    virtual ~ApplicationController() = default;
    virtual void addPoint(int x, int y) = 0;
    virtual void moveHeadPoint(int x, int y) = 0;
    virtual void transformPointsToSnake() = 0;
    virtual void increasePointRadius() = 0;
    virtual void increaseJointDistanceConstraint() = 0;
    virtual void decreasePointRadius() = 0;
    virtual void decreaseJointDistanceConstraint() = 0;
    virtual void increaseMaxAngleConstraint() = 0;
    virtual void decreaseMaxAngleConstraint() = 0;
    virtual void resetApplication() = 0;
};
