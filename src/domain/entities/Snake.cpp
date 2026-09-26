#include "Snake.h"
#include "../../application/use_cases/TranslateJointsUseCase.h"

#include <glm/glm.hpp>


Snake::Snake(std::vector<Joint> circles) : joints_(std::move(circles)) {
    generateEyes();
}

const std::vector<int> &Snake::getIndices() const {
    return indices_;
}

std::unique_ptr<Renderable> Snake::getRenderable() const {
    if (joints_.empty()) {
        return nullptr;
    }
    const auto [vertices, indices] = getJointsConnectedByLine();

    vertices_ = vertices;
    indices_ = indices;
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
    generateEyes();
    const auto [vertices, indices] = getJointsConnectedByLine();
    vertices_ = vertices;
    indices_ = indices;
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
    generateEyes();
    const auto [vertices, indices] = getJointsConnectedByLine();
    vertices_ = vertices;
    indices_ = indices;
}

void Snake::generateEyes() {
    eyes_.clear();
    if (joints_.empty()) {
        return;
    }
    const auto &head = joints_.front();
    glm::vec2 forward(0.0f, 1.0f);
    if (joints_.size() > 1) {
        const auto direction = glm::vec2(head.getTranslation() - joints_[1].getTranslation());
        if (glm::dot(direction, direction) > 0.0f) {
            forward = glm::normalize(direction);
        }
    }
    const glm::vec2 right(forward.y, -forward.x);
    const float eyeRadius = head.getRadius() * 0.18f;
    const float offset = head.getRadius() * 0.55f * glm::cos(glm::radians(45.0f));
    for (const float side : {-1.0f, 1.0f}) {
        const auto center = glm::vec2(head.getTranslation()) + offset * (forward + side * right);
        eyes_.emplace_back(eyeRadius, 0.0f, 0.0f, glm::vec3(1.0f),
                           glm::vec3(center, head.getTranslation().z));
    }
}

std::tuple<std::vector<float>, std::vector<int> > Snake::getJointsConnectedByLine() const {
    std::vector<float> vertices;
    std::vector<int> indices;
    if (joints_.empty()) {
        return {vertices, indices};
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

    const auto getIndicesIndex = [&]() {
        return static_cast<int>(vertices.size()) / 2;
    };

    const auto appendPoint = [&](const std::size_t i, const float x, const float y) {
        const glm::vec2 right(forward[i].y, -forward[i].x);
        const auto point = glm::vec2(joints_[i].getTranslation()) + right * x + forward[i] * y;
        vertices.push_back(point.x);
        vertices.push_back(point.y);
        return point;
    };

    const auto appendArc = [&](const std::size_t i, const std::vector<float> &arc) {
        std::vector<glm::vec2> arcPoints;
        for (std::size_t j = 0; j < arc.size(); j += 2) {
            arcPoints.push_back(appendPoint(i, arc[j], arc[j + 1]));
        }
        if (arcPoints.size() > 3) {
            const auto firstIndex = static_cast<int>(getIndicesIndex() - arcPoints.size());
            for (auto k = firstIndex; k + 2 < getIndicesIndex(); ++k) {
                indices.push_back(firstIndex);
                indices.push_back(k + 1);
                indices.push_back(k + 2);
            }
        }
    };

    if (joints_.size() == 1) {
        appendArc(0, joints_.front().getVertices());
        return {vertices, indices};
    }


    const std::size_t count = joints_.size();

    std::vector<int> leftSide(count);
    std::vector<int> rightSide(count);

    rightSide[0] = getIndicesIndex();

    appendArc(0, joints_.front().getHalfFrontJointVertices());

    leftSide[0] = getIndicesIndex() - 1;
    for (std::size_t i = 1; i + 1 < joints_.size(); ++i) {
        leftSide[i] = getIndicesIndex();
        appendPoint(i, +joints_[i].getVertices()[2], 0.0f);
    }
    leftSide[count - 1] = getIndicesIndex();

    appendArc(joints_.size() - 1, joints_.back().getBackHalfCircleVertices());
    rightSide[count - 1] = getIndicesIndex() - 1;
    for (std::size_t i = joints_.size() - 1; i > 1; --i) {
        const std::size_t jointIndex = i - 1;

        rightSide[jointIndex] = getIndicesIndex();

        appendPoint(jointIndex, -joints_[jointIndex].getVertices()[2], 0.0f);
    }

    for (std::size_t i = 0; i + 1 < leftSide.size(); ++i) {
        const int l0 = leftSide[i];
        const int r0 = rightSide[i];
        const int l1 = leftSide[i + 1];
        const int r1 = rightSide[i + 1];

        indices.push_back(l0);
        indices.push_back(l1);
        indices.push_back(r0);

        indices.push_back(r0);
        indices.push_back(l1);
        indices.push_back(r1);
    }
    return {vertices, indices};
}
