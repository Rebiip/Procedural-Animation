#pragma once
#include <vector>
#include <memory>
#include <utility>

#include "Joint.h"
#include "../../domain/entities/Renderable.h"

class Snake : public Renderable {
public:
    Snake() = default;

    explicit Snake(std::vector<Joint> circles) : joints_(std::move(circles)) {
    }

    [[nodiscard]] const std::vector<float> &getVertices() const override { return vertices_; }
    [[nodiscard]] glm::vec3 getColor() const override { return glm::vec3(0.0f); }
    [[nodiscard]] glm::vec3 getTranslation() const override { return glm::vec3(0.0f); }
    [[nodiscard]] Primitive getPrimitive() const override { return Primitive::TriangleFan; }


    ~Snake() override = default;


    const std::vector<int> &getIndices() const override;

    [[nodiscard]] std::unique_ptr<Renderable> getRenderable() const;

    void translateTo(float xPos, float yPos) override;

    void setDestination(int x_pos, int y_pos);

    void update(float delta_time);

private:
    std::vector<Joint> joints_{};
    mutable std::vector<float> vertices_{};
    std::vector<int> indices_{};


    [[nodiscard]] std::vector<float> getJointsConnectedByLine() const;
};
