#pragma once
#include <vector>
#include <memory>

#include "../../domain/entities/Renderable.h"

struct Scene {
    std::vector<std::unique_ptr<Renderable>> renderables;
};
