#pragma once

#include "../../application/ports/ApplicationController.h"
#include "../../domain/entities/Window.h"
#include "../../presentation/ProcessInputController.h"
#include "../../infrastructure/opengl/OpenGlRenderer.h"

class OpenGlApplicationController : public ApplicationController {
public:
    OpenGlApplicationController(Window &window, ProcessInputController processInputController);

    void run(Scene &scene) override;

private:
    Window &window_;
    ProcessInputController processInputController_;
    OpenGlRenderer renderer_;
};
