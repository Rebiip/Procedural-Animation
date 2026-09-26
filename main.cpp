#include "src/application/SceneController.h"
#include "src/infrastructure/opengl/GlfwInputAdapter.h"
#include "src/infrastructure/opengl/OpenGlApplication.h"
#include "src/infrastructure/opengl/OpenGlRenderer.h"
#include "src/infrastructure/opengl/OpenGlWindow.h"
#include "src/presentation/ProcessInputController.h"

int main() {
    Scene scene;
    SceneController sceneController(scene);
    ProcessInputController inputController(sceneController);

    OpenGlWindow window(720, 480, "Procedural Learning");
    window.create();
    window.initializeDefaults();

    OpenGlRenderer renderer(window);
    GlfwInputAdapter inputAdapter(window, inputController);
    const OpenGlApplication application(window, renderer);
    application.run(scene);
    return 0;
}
