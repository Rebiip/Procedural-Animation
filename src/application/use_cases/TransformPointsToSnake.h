#pragma once
#include <vector>
#include <glm/vec3.hpp>

#include "../../domain/entities/Joint.h"
#include "../../domain/entities/Renderable.h"


class TransformPointsToSnake {
public:
    static void execute(std::vector<std::unique_ptr<Renderable> >& renderables);
private:

    static bool isAllCircle(const std::vector<std::unique_ptr<Renderable>> & renderables);

    static std::vector<Joint> toCircle(const std::vector<std::unique_ptr<Renderable>> & vector);

};
