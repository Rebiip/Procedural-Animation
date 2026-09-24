#pragma once
#include <functional>
#include <GLFW/glfw3.h>

#include "../../domain/entities/Window.h"

class OpenGlWindow : public Window {
public:
    OpenGlWindow(int width, int height, const char *title);

    ~OpenGlWindow() override;

    void create() override;

    void destroy() override;

    void initializeDefaults() override;

    bool shouldClose() override;

    void swapBuffers() override;

    void getSize(int *width, int *height) override;

    void getFramebufferSize(int *width, int *height) override;

    void getCursorPosition(double *x, double *y) override;

    void setMouseButtonCallback(
        std::function<void(Window *window, int button, int action, int mods)> callback) override;

    void setKeyboardButtonCallback(
        std::function<void(Window *window, int key, int scancode, int action, int mods)> callback) override;

private:
    int width_;
    int height_;
    const char *title_;
    std::function<void(Window *window, int, int, int)> mouseCallback_ = nullptr;
    std::function<void(Window *window, int, int, int, int)> keyboardCallback_ = nullptr;
    GLFWwindow *windowHandle_ = nullptr;

    static void resizeWindow(GLFWwindow *, int width, int height);

    static void glfwMouseButtonDispatcher(GLFWwindow *, int button, int action, int mods);

    static void glfwKeyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};
