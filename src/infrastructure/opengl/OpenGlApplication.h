#pragma once
#include <map>
#include <freetype/freetype.h>

#include "../../application/dtos/Scene.h"
#include "../../application/ports/Renderer.h"
#include "../../domain/entities/Window.h"
#include "../../domain/entities/Character.h"

class OpenGlApplication {
public:
    OpenGlApplication(Window &window, Renderer &renderer);

    void run(const Scene &scene) const;

private:
    Window &window_;
    Renderer &renderer_;
    std::map<char, Character> characters_;
    FT_Library ft_;
    FT_Face face_;

    void initializeCharactersMap();
};
