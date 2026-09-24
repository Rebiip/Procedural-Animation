#pragma once
#include <vector>

#include "../../domain/entities/Renderable.h"

class MoveHeadPoint {
public:
    static void execute(const std::vector<std::unique_ptr<Renderable>>& renderables, int xPos, int yPos);
};
