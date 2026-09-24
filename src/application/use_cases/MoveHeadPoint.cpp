#include "MoveHeadPoint.h"

#include "../../domain/entities/Joint.h"
#include "../../domain/entities/Snake.h"

void MoveHeadPoint::execute(const std::vector<std::unique_ptr<Renderable> > &renderables, const int xPos,
                            const int yPos) {
    if (renderables.empty()) {
        return;
    }
    if (const auto circle = dynamic_cast<Joint *>(renderables.at(0).get())) {
        circle->setDestination(xPos, yPos);
    }
    if (const auto snake = dynamic_cast<Snake *>(renderables.at(0).get())) {
        snake->setDestination(xPos, yPos);
    }
}
