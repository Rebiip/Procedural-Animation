#pragma once
#include <functional>

class Window {
public:
    Window() = default;

    virtual ~Window() = default;

    virtual void create() = 0;

    virtual void destroy() = 0;

    virtual void initializeDefaults() = 0;

    virtual bool shouldClose() = 0;

    virtual void swapBuffers() = 0;

    virtual void getSize(int *width, int *height) = 0;

    virtual void getFramebufferSize(int *width, int *height) = 0;

    virtual void getCursorPosition(double *x, double *y) = 0;

    virtual void setMouseButtonCallback(std::function<void(Window *window, int button, int action, int mods)> callback)
    = 0;

    virtual void setKeyboardButtonCallback(
        std::function<void(Window *window, int key, int scancode, int action, int mods)> callback) = 0;
};
