#include "OpenGlRenderer.h"
#include "../../domain/entities/Snake.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <iostream>
#include <ranges>

#include FT_FREETYPE_H


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

    glGenVertexArrays(1, &textVertexArray_);
    glGenBuffers(1, &textVertexBuffer_);
    glBindVertexArray(textVertexArray_);
    glBindBuffer(GL_ARRAY_BUFFER, textVertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (FT_Init_FreeType(&ft_)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    if (FT_New_Face(ft_, "/Users/alexandre.marinho/Library/Fonts/HackNerdFont-Regular.ttf", 0, &face_)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    FT_Set_Pixel_Sizes(face_, 0, 48);
    initializeCharactersMap();

    textShader_ = std::make_unique<Shader>(SHADER_DIRECTORY "/text.shader.vs",
                                           SHADER_DIRECTORY "/text.shader.fs");
}

OpenGlRenderer::~OpenGlRenderer() {
    glDeleteBuffers(1, &textVertexBuffer_);
    glDeleteVertexArrays(1, &textVertexArray_);
    for (const auto &glyph: characters_ | std::views::values) {
        glDeleteTextures(1, &glyph.TextureID);
    }
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

void OpenGlRenderer::renderText(const std::string &text, float x, const float y, const float scale,
                                const glm::vec3 &color) {
    int width;
    int height;
    window_.getSize(&width, &height);
    const auto depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    textShader_->use();
    textShader_->setMat4("projection", glm::ortho(
        0.0f, static_cast<float>(std::max(width, 1)),
        0.0f, static_cast<float>(std::max(height, 1))));
    glUniform1i(glGetUniformLocation(textShader_->ID, "text"), 0);
    glUniform3f(glGetUniformLocation(textShader_->ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVertexArray_);

    for (char c: text) {
        const auto glyph = characters_.find(c);
        if (glyph == characters_.end()) {
            continue;
        }
        const auto &[TextureID, Size, Bearing, Advance] = glyph->second;
        if (Size.x == 0 || Size.y == 0) {
            x += static_cast<float>(Advance >> 6) * scale;
            continue;
        }

        const float xpos = x + static_cast<float>(Bearing.x) * scale;
        const float ypos = y - static_cast<float>(Size.y - Bearing.y) * scale;

        const float w = static_cast<float>(Size.x) * scale;
        const float h = static_cast<float>(Size.y) * scale;
        const float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}
        };
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, textVertexBuffer_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += static_cast<float>(Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (depthTestEnabled) {
        glEnable(GL_DEPTH_TEST);
    }
}

void OpenGlRenderer::initializeCharactersMap() {
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
            static_cast<int>(face_->glyph->bitmap.width),
            static_cast<int>(face_->glyph->bitmap.rows),
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
            .TextureID = texture,
            .Size = glm::ivec2(face_->glyph->bitmap.width, face_->glyph->bitmap.rows),
            .Bearing = glm::ivec2(face_->glyph->bitmap_left, face_->glyph->bitmap_top),
            .Advance = static_cast<unsigned int>(face_->glyph->advance.x)
        };
        characters_.insert(std::pair<char, Character>(c, character));
    }
    FT_Done_Face(face_);
    FT_Done_FreeType(ft_);
}

void OpenGlRenderer::init() {
    gladInit();
}

void OpenGlRenderer::gladInit() {
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
