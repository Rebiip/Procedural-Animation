#include "OpenGlRenderer.h"
#include "../../domain/entities/Snake.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <limits>
#include <stdexcept>


OpenGlRenderer::OpenGlRenderer(Window &window) : window_(window) {
    init();
    shader_ = std::make_unique<Shader>(SHADER_DIRECTORY "/object.shader.vs",
                                       SHADER_DIRECTORY "/object.shader.fs");
    glGenVertexArrays(1, &vertexArray_);
    glGenBuffers(1, &vertexBuffer_);
    glGenBuffers(1, &indexBuffer_);
    glBindVertexArray(vertexArray_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGlRenderer::~OpenGlRenderer() {
    glDeleteBuffers(1, &vertexBuffer_);
    glDeleteVertexArrays(1, &vertexArray_);
    glDeleteBuffers(1, &indexBuffer_);
}

void OpenGlRenderer::beginFrame(const Camera &camera) {
    int width;
    int height;
    int framebufferWidth;
    int framebufferHeight;
    window_.getSize(&width, &height);
    window_.getFramebufferSize(&framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const auto viewportWidth = static_cast<float>(std::max(width, 1));
    const auto viewportHeight = static_cast<float>(std::max(height, 1));
    const glm::mat4 projection = camera.orthographic
                                     ? glm::ortho(0.0f, viewportWidth, viewportHeight, 0.0f, -camera.farPlane,
                                                  camera.farPlane)
                                     : glm::perspective(glm::radians(camera.fov), viewportWidth / viewportHeight,
                                                        camera.nearPlane, camera.farPlane);
    if (camera.orthographic) {
        glDisable(GL_DEPTH_TEST);
    } else {
        glEnable(GL_DEPTH_TEST);
    }
    shader_->use();
    shader_->setMat4("view", glm::lookAt(camera.position, camera.position + camera.forward, camera.up));
    shader_->setMat4("projection", projection);
}

void OpenGlRenderer::render(const Renderable &renderable) {
    const auto &vertices = renderable.getVertices();
    if (vertices.empty()) {
        return;
    }
    if (vertices.size() % 2 != 0 || vertices.size() < 6 ||
        vertices.size() / 2 > static_cast<std::size_t>(std::numeric_limits<GLsizei>::max())) {
        throw std::invalid_argument("Renderable must contain at least three packed x/y vertices");
    }
    shader_->use();
    shader_->setMat4("model", glm::translate(glm::mat4(1.0f), renderable.getTranslation()));
    shader_->setVec3("objectColor", renderable.getColor());
    draw(renderable);
    if (const auto *snake = dynamic_cast<const Snake *>(&renderable)) {
        for (const auto &eye: snake->getEyes()) {
            render(eye);
        }
    }
}

void OpenGlRenderer::endFrame() {
    window_.swapBuffers();
}

void OpenGlRenderer::init() const {
    gladInit();
}

void OpenGlRenderer::gladInit() const {
    if (!gladLoadGL(glfwGetProcAddress)) {
        throw std::invalid_argument("OpenGL functions could not be loaded");
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGlRenderer::draw(const Renderable &renderable) const {
    const auto &vertices = renderable.getVertices();
    const auto &indices = renderable.getIndices();
    glBindVertexArray(vertexArray_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
                 vertices.data(), GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
                 indices.data(), GL_STREAM_DRAW);
    const auto primitive = renderable.getPrimitive() == Renderable::Primitive::LineLoop
                               ? GL_LINE_LOOP
                               : GL_TRIANGLES;
    glDrawElements(primitive, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
