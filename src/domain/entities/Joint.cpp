#include "Joint.h"

#include <iostream>
#include <glm/glm.hpp>

Joint::Joint(const float radius, const float anchorRadius, const float restrictionAngle, const glm::vec3 color,
             const glm::vec3 translation)
    : radius_(radius), anchorRadius_(anchorRadius), restrictionAngle_(restrictionAngle), color_(color),
      translation_(translation) {
    auto center = glm::vec2(0.0f);
    vertices_.reserve(36 * 2 + 2);
    vertices_.push_back(center.x);
    vertices_.push_back(center.y);
    int circle_vertices = 36;
    for (int i = 0; i < circle_vertices; ++i) {
        const float angle = glm::radians(static_cast<float>(i) * 10.0f);
        vertices_.push_back(radius_ * cos(angle));
        vertices_.push_back(radius_ * sin(angle));
        indices_.push_back(0);
        if (i + 1 == circle_vertices) {
            indices_.push_back(1);
            indices_.push_back(i + 1);
        } else {
            indices_.push_back(i + 1);
            indices_.push_back(i + 2);
        }
    }
    destination_.x = translation_.x;
    destination_.y = translation_.y;
}

const std::vector<float> &Joint::getVertices() const {
    return vertices_;
}

const std::vector<int> &Joint::getIndices() const {
    return indices_;
}

glm::vec3 Joint::getColor() const {
    return color_;
}

glm::vec3 Joint::getTranslation() const {
    return translation_;
}


void Joint::translateTo(const float xPos, const float yPos) {
    const glm::vec3 targetPosition(xPos, yPos, 0.0f);
    const glm::vec3 offset = targetPosition - translation_;
    if (const auto length = glm::length(offset); length > 0.0f) {
        const auto direction = glm::normalize(offset);
        translation_ += direction * length;
    }
}

void Joint::setDestination(const int x_pos, const int y_pos) {
    destination_ = glm::vec2(x_pos, y_pos);
}

std::vector<float> Joint::getHalfFrontJointVertices() const {
    std::vector<float> frontHalfVertices;
    for (int i = 0; i <= 18; ++i) {
        const float angle = glm::radians(static_cast<float>(i) * 10.0f);
        frontHalfVertices.push_back(radius_ * cos(angle));
        frontHalfVertices.push_back(radius_ * sin(angle));
    }
    return frontHalfVertices;
}

std::vector<float> Joint::getBackHalfCircleVertices() const {
    std::vector<float> backHalfVertices;
    for (int i = 18; i <= 36; ++i) {
        const float angle = glm::radians(static_cast<float>(i) * 10.0f);
        backHalfVertices.push_back(radius_ * cos(angle));
        backHalfVertices.push_back(radius_ * sin(angle));
    }
    return backHalfVertices;
}

float Joint::getAnchorRadius() const {
    return anchorRadius_;
}


bool Joint::isOutsideAnchorLine(const glm::vec3 *value) const {
    return glm::distance(translation_, *value) > anchorRadius_;
}
