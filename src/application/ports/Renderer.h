#pragma once
#include "../../domain/entities/Camera.h"
#include "../../domain/entities/Renderable.h"

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual void beginFrame(const Camera &) = 0;

    virtual void render(const Renderable &) = 0;

    virtual void endFrame() = 0;

};
