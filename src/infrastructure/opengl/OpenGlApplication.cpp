#include "OpenGlApplication.h"
#include <GLFW/glfw3.h>


#include "../../application/use_cases/UpdateScene.h"


OpenGlApplication::OpenGlApplication(Window &window,
                                     Renderer &renderer) : window_(window),
                                                           renderer_(renderer) {
}

void OpenGlApplication::run(const Scene &scene) const {
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
