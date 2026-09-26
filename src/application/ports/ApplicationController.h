#pragma once

// Input boundary for scene commands; independent of the window and render loop.
class ApplicationController {
public:
    virtual ~ApplicationController() = default;
    virtual void addPoint(int x, int y) = 0;
    virtual void moveHeadPoint(int x, int y) = 0;
    virtual void transformPointsToSnake() = 0;
    virtual void increasePointRadius() = 0;
};
