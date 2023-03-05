#include <Disaster/Graphics/Transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace px::disaster {
  Transform::Transform() : m_transform(1.0f) {}

  void Transform::SetPosition(Vector2f position) {
    SetPosition(position.x, position.y);
  }
  void Transform::SetPosition(float x, float y) {
    m_transform[3].x = x;
    m_transform[3].y = y;
    m_transform[3].z = 0.0f;
  }

  Vector2f Transform::GetPosition() const {
    glm::vec3 pos = glm::vec3(m_transform[3]);
    return Vector2f(pos.x, pos.y);
  }

  void Transform::Move(Vector2f offset) {
    Move(offset.x, offset.y);
  }
  void Transform::Move(float x, float y) {
    m_transform = glm::translate(m_transform, glm::vec3(x, y, 0.0f));
  }

  glm::mat4 Transform::GetMat4() const {
    return m_transform;
  }
  const glm::mat4 *Transform::GetMat4Ptr() const {
    return &m_transform;
  }
}