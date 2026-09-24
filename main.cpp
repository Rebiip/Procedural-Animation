#include "src/infrastructure/opengl/OpenGlRenderer.h"
#include "src/infrastructure/opengl/OpenGlWindow.h"
#include "src/application/dtos/Scene.h"
#include "src/infrastructure/opengl/OpenGlApplicationController.h"

int main() {
    OpenGlWindow window(720, 480, "Procedural Learning");
    Scene scene;
    const auto processInputController = ProcessInputController(scene);
    OpenGlApplicationController controller(window, processInputController);
    controller.run(scene);
    return 0;
}
