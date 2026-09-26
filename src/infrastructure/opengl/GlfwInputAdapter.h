#pragma once
#include "../../domain/entities/Window.h"
#include "../../presentation/ProcessInputController.h"


class GlfwInputAdapter {
public:
    GlfwInputAdapter(Window &window, ProcessInputController &processInputController);
    ~GlfwInputAdapter();

    GlfwInputAdapter(const GlfwInputAdapter &) = delete;
    GlfwInputAdapter &operator=(const GlfwInputAdapter &) = delete;

    void keyboardCallback(Window *window, int key, int scancode, int action, int mods) const;

    void mouseCallback(Window *, int button, int action, int mods) const;

private:
    Window &window_;
    ProcessInputController &controller_;
};
