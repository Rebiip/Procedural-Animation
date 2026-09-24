#pragma once
#include "../../domain/entities/Window.h"
#include "../../presentation/ProcessInputController.h"


class GlfwInputAdapter {
public:
    explicit GlfwInputAdapter(ProcessInputController &processInputController);

    void keyboardCallback(Window *window, int key, int scancode, int action, int mods) const;

    void mouseCallback(Window *, int button, int action, int mods) const;

private:
    ProcessInputController &controller_;
};
