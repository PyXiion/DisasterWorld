#include <Disaster/Gameplay/Camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace px::disaster::gameplay {
  Camera::Camera() 
    : m_zoom(1.0f) {
      m_projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, 0.1f, 100.0f);
      Update();
    }
  
  void Camera::Update() {
    // sf::Vector2u windowSize = m_window.getSize();
    // m_view.setSize(windowSize.x / m_zoom, windowSize.y / m_zoom);
    // m_view.setCenter(m_position);
    m_view = glm::lookAt(glm::vec3(m_position.x, m_position.y, 0.0f),
                          glm::vec3(0.0f, 0.0f, 0.0f),
                          glm::vec3(0.0f, 1.0f, 0.0f));

    // m_window.setView(m_view);
  }

  void Camera::Move(Vector2f offset) {
    m_position += offset;
  }
  void Camera::Zoom(float factor) {
    m_zoom *= factor;
  }

  void Camera::SetPosition(Vector2f position) {
    m_position = position;
  }
  void Camera::SetZoom(float zoom) {
    m_zoom = zoom;
  }

  Vector2f Camera::GetSize() const {
    // return m_view.getSize();
    return Vector2f();
  }
  Vector2f Camera::GetPosition() const {
    return m_position;
  }
  float Camera::GetZoom() const {
    return m_zoom;
  }
}