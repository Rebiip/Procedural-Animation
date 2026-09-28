#include "OpenGlWindow.h"

#include <stdexcept>
#include "GLFW/glfw3.h"

#include <ft2build.h>


OpenGlWindow::OpenGlWindow(const int width, const int height, const char *title) {
    width_ = width;
    height_ = height;
    title_ = title;
}

void OpenGlWindow::create() {
    if (!glfwInit()) {
        throw std::domain_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    windowHandle_ = glfwCreateWindow(width_, height_, title_, nullptr, nullptr);
    if (!windowHandle_) {
        glfwTerminate();
        throw std::domain_error("Failed to create GLFW window");
    }
    glfwSetWindowUserPointer(windowHandle_, this);
    glfwMakeContextCurrent(windowHandle_);
    glfwSwapInterval(1);

}

void OpenGlWindow::destroy() {
    if (windowHandle_) {
        glfwDestroyWindow(windowHandle_);
        windowHandle_ = nullptr;
        glfwTerminate();
    }
}

OpenGlWindow::~OpenGlWindow() {
    OpenGlWindow::destroy();
}

void OpenGlWindow::getSize(int *width, int *height) {
    glfwGetWindowSize(windowHandle_, width, height);
}

void OpenGlWindow::getFramebufferSize(int *width, int *height) {
    glfwGetFramebufferSize(windowHandle_, width, height);
}

void OpenGlWindow::initializeDefaults() {
    glfwSetFramebufferSizeCallback(windowHandle_, resizeWindow);
    glfwSetInputMode(windowHandle_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetMouseButtonCallback(windowHandle_, glfwMouseButtonDispatcher);
    glfwSetKeyCallback(windowHandle_, glfwKeyboardCallback);
}

bool OpenGlWindow::shouldClose() {
    return glfwWindowShouldClose(windowHandle_) == GLFW_TRUE;
}

void OpenGlWindow::swapBuffers() {
    glfwSwapBuffers(windowHandle_);
}

void OpenGlWindow::getCursorPosition(double *x, double *y) {
    glfwGetCursorPos(windowHandle_, x, y);
}

void OpenGlWindow::setMouseButtonCallback(
    std::function<void(Window *window, int button, int action, int mods)> callback) {
    mouseCallback_ = callback;
}

void OpenGlWindow::setKeyboardButtonCallback(
    const std::function<void(Window *window, int key, int scancode, int action, int mods)> callback) {
    keyboardCallback_ = callback;
}


void OpenGlWindow::resizeWindow(GLFWwindow *, const int width, const int height) {
    glViewport(0, 0, width, height);
}

void OpenGlWindow::glfwMouseButtonDispatcher(GLFWwindow *window, const int button, const int action, const int mods) {
    auto *instance =
            static_cast<OpenGlWindow *>(glfwGetWindowUserPointer(window));
    if (instance && instance->mouseCallback_) {
        instance->mouseCallback_(instance, button, action, mods);
    }
}

void OpenGlWindow::glfwKeyboardCallback(GLFWwindow *window, const int key, const int scancode, const int action,
                                        const int mods) {
    auto *instance =
            static_cast<OpenGlWindow *>(glfwGetWindowUserPointer(window));
    if (instance && instance->keyboardCallback_) {
        instance->keyboardCallback_(instance, key, scancode, action, mods);
    }
}
