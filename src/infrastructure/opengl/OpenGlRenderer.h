#pragma once
#include <memory>
#include "shaders/Shader.h"
#include <map>


#include "../../application/ports/Renderer.h"
#include "../../domain/entities/Renderable.h"
#include "../../domain/entities/Window.h"
#include "../../domain/entities/Camera.h"
#include "../../domain/entities/Character.h"

#include <freetype/freetype.h>


class OpenGlRenderer : public Renderer {
public:
    explicit OpenGlRenderer(Window &window);

    ~OpenGlRenderer() override;

    void beginFrame(const Camera &) override;

    void render(const Renderable &) override;

    void endFrame() override;

    void renderText(const std::string &text, float x, float y, float scale, const glm::vec3 &color) override;

    void initializeCharactersMap();

private:
    Window &window_;
    std::unique_ptr<Shader> shader_;
    std::unique_ptr<Shader> textShader_;
    std::map<char, Character> characters_;
    FT_Library ft_{};
    FT_Face face_{};
    unsigned int vertexArray_ = 0;
    unsigned int vertexBuffer_ = 0;
    unsigned int indexBuffer_ = 0;
    unsigned int textVertexArray_ = 0;
    unsigned int textVertexBuffer_ = 0;

    static void init();

    static void gladInit();

    void draw(const Renderable &renderable) const;
};
