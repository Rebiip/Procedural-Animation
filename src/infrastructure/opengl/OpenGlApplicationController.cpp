#include "OpenGlApplicationController.h"
#include <GLFW/glfw3.h>
#include "../../application/use_cases/UpdateScene.h"

OpenGlApplicationController::
OpenGlApplicationController(Window &window, const ProcessInputController processInputController) : window_(window),
    processInputController_(processInputController), renderer_(window, processInputController_) {
}

void OpenGlApplicationController::run(Scene &scene) {
    auto lastTime = glfwGetTime();
    while (!window_.shouldClose()) {
        renderer_.beginFrame(Camera{});
        for (const auto &renderable: scene.renderables) {
            renderer_.render(*renderable);
        }
        renderer_.endFrame();
        glfwPollEvents();
        const auto currentTime = glfwGetTime();
        const auto deltaTime = static_cast<float>(currentTime - lastTime) * 3.3f;
        UpdateScene::execute(scene, deltaTime);
        lastTime = currentTime;
    }
}
