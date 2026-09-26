#pragma once
#include "./dtos/MouseClickEvent.h"
#include "../application/ports/ApplicationController.h"

class ProcessInputController {
public:
    explicit ProcessInputController(ApplicationController &applicationController);

    void onMouseClick(MouseClickEvent event) const;

    void onButtonClick(int button) const;

private:
    ApplicationController &applicationController_;
};
