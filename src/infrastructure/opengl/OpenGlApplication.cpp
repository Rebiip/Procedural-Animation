#include "OpenGlApplication.h"
#include <GLFW/glfw3.h>


#include "../../application/use_cases/UpdateScene.h"
#include <ft2build.h>
#include <iostream>

#include FT_FREETYPE_H


OpenGlApplication::OpenGlApplication(Window &window,
                                     Renderer &renderer) : window_(window),
                                                           renderer_(renderer) {
    if (FT_Init_FreeType(&ft_)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    if (FT_New_Face(ft_, "/Users/alexandre.marinho/Library/Fonts/HackNerdFont-Regular.ttf", 0, &face_)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    FT_Set_Pixel_Sizes(face_, 0, 48);
    initializeCharactersMap();
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

void OpenGlApplication::initializeCharactersMap() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face_, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face_->glyph->bitmap.width,
            face_->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face_->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
            texture,
            glm::ivec2(face_->glyph->bitmap.width, face_->glyph->bitmap.rows),
            glm::ivec2(face_->glyph->bitmap_left, face_->glyph->bitmap_top),
            static_cast<unsigned int>(face_->glyph->advance.x)
        };
        characters_.insert(std::pair<char, Character>(c, character));
    }
    FT_Done_Face(face_);
    FT_Done_FreeType(ft_);
}
