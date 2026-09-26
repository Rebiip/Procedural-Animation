#pragma once
#include <memory>
#include "shaders/Shader.h"

#include "../../application/ports/Renderer.h"
#include "../../domain/entities/Renderable.h"
#include "../../domain/entities/Window.h"
#include "../../domain/entities/Camera.h"


class OpenGlRenderer : public Renderer {
public:
    explicit OpenGlRenderer(Window &window);

    ~OpenGlRenderer() override;

    void beginFrame(const Camera &) override;

    void render(const Renderable &) override;

    void endFrame() override;

private:
    Window &window_;
    std::unique_ptr<Shader> shader_;
    unsigned int vertexArray_ = 0;
    unsigned int vertexBuffer_ = 0;
    unsigned int indexBuffer_ = 0;

    void init() const;

    void gladInit() const;

    void draw(const Renderable &renderable) const;
};
