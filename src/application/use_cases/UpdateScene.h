#pragma once
#include "../dtos/Scene.h"

class UpdateScene {
public:
    static void execute(const Scene &scene, float deltaTime);
};
