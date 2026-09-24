#pragma once
#include "./dtos/MouseClickEvent.h"
#include "../application/dtos/Scene.h"
class ProcessInputController {
public:
    explicit ProcessInputController(Scene &scene);

    void onMouseClick(MouseClickEvent event) const;
    void onButtonClick(int button) const;

private:
    Scene &scene_;
};
