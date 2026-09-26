#pragma once
#include "../../application/dtos/Scene.h"
#include "../../application/ports/Renderer.h"
#include "../../domain/entities/Window.h"

class OpenGlApplication {
public:
    OpenGlApplication(Window &window, Renderer &renderer);

    void run(const Scene &scene) const;

private:
    Window &window_;
    Renderer &renderer_;
};
