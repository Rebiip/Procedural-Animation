#include "GlfwInputAdapter.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include "../../presentation/dtos/MouseButton.h"

GlfwInputAdapter::GlfwInputAdapter(Window &window, ProcessInputController &processInputController)
    : window_(window), controller_(processInputController) {
    window_.setMouseButtonCallback(
        [this](Window *source, int button, int action, int mods) {
            mouseCallback(source, button, action, mods);
        });
    window_.setKeyboardButtonCallback(
        [this](Window *source, int key, int scancode, int action, int mods) {
            keyboardCallback(source, key, scancode, action, mods);
        });
}

GlfwInputAdapter::~GlfwInputAdapter() {
    window_.setMouseButtonCallback({});
    window_.setKeyboardButtonCallback({});
}

void GlfwInputAdapter::keyboardCallback(Window *window, const int key, int scancode, const int action,
                                        const int mods) const {
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
