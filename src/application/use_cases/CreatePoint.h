#pragma once
#include <memory>
#include "../../domain/entities/Renderable.h"
class CreatePoint {

public:
    static std::unique_ptr<Renderable> execute(int xPos, int yPos);
};

