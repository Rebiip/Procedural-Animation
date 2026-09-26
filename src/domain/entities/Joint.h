#pragma once
#include <glm/vec2.hpp>

#include "Renderable.h"

class Joint : public Renderable {
public:
    explicit Joint(float radius, float anchorRadius, float restrictionAngle, glm::vec3 color = glm::vec3(0.0f),
                   glm::vec3 translation = glm::vec3(0.0f));

    [[nodiscard]] const std::vector<float> &getVertices() const override;

    [[nodiscard]] const std::vector<int> &getIndices() const override;

    [[nodiscard]] glm::vec3 getColor() const override;

    [[nodiscard]] glm::vec3 getTranslation() const override;

    void translateTo(float xPos, float yPos) override;

    void setDestination(int x_pos, int y_pos);

    [[nodiscard]] std::vector<float> getHalfFrontJointVertices() const;

    [[nodiscard]] std::vector<float> getBackHalfCircleVertices() const;

    [[nodiscard]] float getAnchorRadius() const;
    [[nodiscard]] float getRadius() const { return radius_; }
    [[nodiscard]] float getRestrictionAngle() const { return restrictionAngle_; }
    [[nodiscard]] glm::vec3 getDestination() const { return glm::vec3(destination_, 0.0f); }

private:
    float radius_;
    float anchorRadius_;
    float restrictionAngle_;

    glm::vec3 color_;
    glm::vec3 translation_;
    glm::vec2 destination_ = {};

    std::vector<float> vertices_{};
    std::vector<int> indices_{};


    bool isOutsideAnchorLine(const glm::vec3 *value) const;
};
