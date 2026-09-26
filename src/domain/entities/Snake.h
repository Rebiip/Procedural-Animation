#pragma once
#include <vector>
#include <memory>
#include <utility>

#include "Joint.h"
#include "../../domain/entities/Renderable.h"

class Snake : public Renderable {
public:
    Snake() = default;

    explicit Snake(std::vector<Joint> circles);

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

    [[nodiscard]] const std::vector<Joint> &getEyes() const { return eyes_; }

private:
    std::vector<Joint> joints_{};
    std::vector<Joint> eyes_{};
    mutable std::vector<float> vertices_{};
    mutable std::vector<int> indices_{};

    void generateEyes();


    [[nodiscard]] std::tuple<std::vector<float>, std::vector<int> > getJointsConnectedByLine() const;
};
