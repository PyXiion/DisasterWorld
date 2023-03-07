#include <Disaster/Graphics/Transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace px::disaster {
  Transform::Transform() : m_size(1.0f, 1.0f), m_position(0.0f, 0.0f) {}

  void Transform::SetPosition(Vector2f position) {
    SetPosition(position.x, position.y);
  }
  void Transform::SetPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
  }

  Vector2f Transform::GetPosition() const {
    return m_position;
  }

  void Transform::Move(Vector2f offset) {
    Move(offset.x, offset.y);
  }
  void Transform::Move(float x, float y) {
    m_position.x += x;
    m_position.y += y;
  }

  void Transform::SetSize(Vector2f size) {
    m_size = size;
  }
  void Transform::SetSize(float x, float y) {
    m_size.x = x;
    m_size.y = y;
  }
  Vector2f Transform::GetSize() const {
    return m_size;
  }

  void Transform::Scale(Vector2f scale) {
    m_size.x *= scale.x;
    m_size.y *= scale.y;
  }
  void Transform::Scale(float factor) {
    m_size *= factor;
  }

  glm::mat4 Transform::GetMat4() const {
    glm::mat4 mat4(1.0f);
    mat4 = glm::translate(mat4, glm::vec3(m_position.x, m_position.y, 0.0f));
    mat4 = glm::scale(mat4, glm::vec3(m_size.x, m_size.y, 1.0f));
    return mat4;
  }
}