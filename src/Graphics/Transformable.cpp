#include <Disaster/Graphics/Transformable.hpp>

namespace px::disaster {
  void Transformable::Move(Vector2f offset) {
    m_transform.Move(offset);
  }
  void Transformable::Move(float x, float y) {
    m_transform.Move(x, y);
  }
  void Transformable::SetPosition(Vector2f position) {
    m_transform.SetPosition(position);
  }
  void Transformable::SetPosition(float x, float y) {
    m_transform.SetPosition(x, y);
  }
  Vector2f Transformable::GetPosition() const {
    return m_transform.GetPosition();
  }

  void Transformable::Scale(float factor) {
    m_transform.Scale(factor);
  }
  void Transformable::SetSize(Vector2f size) {
    m_transform.SetSize(size);
  }
  void Transformable::SetSize(float x, float y) {
    m_transform.SetSize(x, y);
  }
  Vector2f Transformable::GetSize() const {
    return m_transform.GetSize();
  }

  Transform &Transformable::GetTransform() {
    return m_transform;
  }
  const Transform &Transformable::GetTransform() const {
    return m_transform;
  }
}