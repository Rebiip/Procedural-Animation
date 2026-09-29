#include "OpenGlApplication.h"
#include <GLFW/glfw3.h>


#include "../../application/use_cases/UpdateScene.h"
#include <iostream>


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
        renderer_.renderText("Joint SIZE: " + std::to_string(scene.jointSize), 10.0f, 10.0f, 0.5f,
                             glm::vec3(0.0f, 0.0f, 0.0f));
        renderer_.renderText("Joint Distance Constraint: " + std::to_string(scene.jointDistanceConstraintSize), 10.0f,
                             30.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
        renderer_.renderText("Max Angle Constraint: " + std::to_string(scene.maxAngleConstraint), 10.0f,
                             50.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
        renderer_.endFrame();
        glfwPollEvents();
        const auto currentTime = glfwGetTime();
        const auto deltaTime = static_cast<float>(currentTime - lastTime) * 3.3f;
        UpdateScene::execute(scene, deltaTime);
        lastTime = currentTime;
    }
}
