#include "Snake.h"
#include "../../application/use_cases/TranslateJointsUseCase.h"

#include <glm/glm.hpp>


const std::vector<int> &Snake::getIndices() const {
    return indices_;
}

std::unique_ptr<Renderable> Snake::getRenderable() const {
    if (joints_.empty()) {
        return nullptr;
    }
    vertices_ = getJointsConnectedByLine();
    return std::make_unique<Snake>(*this);
}


void Snake::translateTo(const float xPos, const float yPos) {
    if (joints_.empty()) {
        return;
    }
    const auto offset = glm::vec3(xPos, yPos, 0.0f) - joints_.front().getTranslation();
    for (auto &joint: joints_) {
        const auto position = joint.getTranslation() + offset;
        joint.translateTo(position.x, position.y);
    }
    vertices_ = getJointsConnectedByLine();
}

void Snake::setDestination(const int x_pos, const int y_pos) {
    if (!joints_.empty()) {
        joints_.front().setDestination(x_pos, y_pos);
    }
}

void Snake::update(const float delta_time) {
    if (joints_.empty()) {
        return;
    }
    const auto destination = joints_.front().getDestination();
    TranslateJointsUseCase::execute(joints_, &destination, delta_time);
    vertices_ = getJointsConnectedByLine();
}

std::vector<float> Snake::getJointsConnectedByLine() const {
    std::vector<float> vertices;
    if (joints_.empty()) {
        return vertices;
    }
    std::vector forward(joints_.size(), glm::vec2(0.0f, 1.0f));
    for (std::size_t i = 0; i < joints_.size(); ++i) {
        const auto previous = i == 0 ? i : i - 1;
        const auto next = i + 1 < joints_.size() ? i + 1 : i;
        auto direction = glm::vec2(joints_[previous].getTranslation() - joints_[next].getTranslation());
        if (glm::dot(direction, direction) == 0.0f) {
            direction = glm::vec2(joints_[previous].getTranslation() - joints_[i].getTranslation());
        }
        if (glm::dot(direction, direction) > 0.0f) {
            forward[i] = glm::normalize(direction);
        } else if (i > 0) {
            forward[i] = forward[i - 1];
        }
    }

    const auto appendPoint = [&](const std::size_t i, const float x, const float y) {
        const glm::vec2 right(forward[i].y, -forward[i].x);
        const auto point = glm::vec2(joints_[i].getTranslation()) + right * x + forward[i] * y;
        vertices.push_back(point.x);
        vertices.push_back(point.y);
    };
    const auto appendArc = [&](const std::size_t i, const std::vector<float> &arc) {
        for (std::size_t j = 0; j < arc.size(); j += 2) {
            appendPoint(i, arc[j], arc[j + 1]);
        }
    };

    if (joints_.size() == 1) {
        appendArc(0, joints_.front().getVertices());
        return vertices;
    }

    appendArc(0, joints_.front().getHalfFrontJointVertices());
    for (std::size_t i = 1; i + 1 < joints_.size(); ++i) {
        appendPoint(i, -joints_[i].getVertices().front(), 0.0f);
    }

    appendArc(joints_.size() - 1, joints_.back().getBackHalfCircleVertices());
    for (std::size_t i = joints_.size() - 1; i > 1; --i) {
        appendPoint(i - 1, joints_[i - 1].getVertices().front(), 0.0f);
    }
    return vertices;
}
