#pragma once
#include <vector>
#include <memory>

#include "../../domain/entities/Renderable.h"

struct Scene {
    std::vector<std::unique_ptr<Renderable> > renderables;
    float jointSize = 10.0f;
    float jointDistanceConstraintSize = 10.0f;
    float maxAngleConstraint = 60.f;
};
