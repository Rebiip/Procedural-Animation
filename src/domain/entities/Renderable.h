#pragma once
#include <vector>
#include <glm/vec3.hpp>

class Renderable {
public:
    virtual ~Renderable() = default;

    enum class Primitive { TriangleFan, LineLoop };

    [[nodiscard]] virtual Primitive getPrimitive() const { return Primitive::TriangleFan; }

    [[nodiscard]] virtual const std::vector<float> &getVertices() const = 0;

    [[nodiscard]] virtual const std::vector<int> &getIndices() const = 0;

    [[nodiscard]] virtual glm::vec3 getColor() const = 0;

    [[nodiscard]] virtual glm::vec3 getTranslation() const = 0;

    virtual void translateTo(float xPos, float yPos) = 0;
};
