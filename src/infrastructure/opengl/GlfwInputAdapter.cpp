#include "GlfwInputAdapter.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include "../../presentation/dtos/MouseButton.h"

GlfwInputAdapter::GlfwInputAdapter(ProcessInputController &processInputController) : controller_(
    processInputController) {
}

void GlfwInputAdapter::keyboardCallback(Window *window, const int key, int scancode, const int action, const int mods) const{
    if (action != GLFW_PRESS) {
        return;
    }

    if (key == GLFW_KEY_ESCAPE && mods == 0) {
        std::cout << "Escape key pressed" << std::endl;
    }
    controller_.onButtonClick(key);
}

void GlfwInputAdapter::mouseCallback(Window *window, const int button, const int action, int mods) const {
    if (action != GLFW_PRESS) {
        return;
    }

    double x;
    double y;

    window->getCursorPosition(&x, &y);

    MouseButton mappedButton;

    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            mappedButton = MouseButton::Left;
            break;

        case GLFW_MOUSE_BUTTON_RIGHT:
            mappedButton = MouseButton::Right;
            break;

        default:
            return;
    }

    controller_.onMouseClick({
        .button = mappedButton,
        .x = static_cast<float>(x),
        .y = static_cast<float>(y)
    });
}
