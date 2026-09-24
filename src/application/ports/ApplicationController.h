#pragma once
#include "../dtos/Scene.h"

class ApplicationController {
public:
    virtual void run(Scene &scene) = 0;
    virtual ~ApplicationController() = default;

};
