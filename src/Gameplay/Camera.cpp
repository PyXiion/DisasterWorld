#include <Disaster/Gameplay/Camera.hpp>

namespace px::disaster::gameplay {
  Camera::Camera(sf::RenderWindow &window) 
    : m_window(window), m_view(window.getDefaultView()), m_zoom(1.0f) {}
  
  void Camera::Update(float deltaTime) {
    sf::Vector2u windowSize = m_window.getSize();
    m_view.setSize(windowSize.x / m_zoom, windowSize.y / m_zoom);
    m_view.setCenter(m_position);
    m_window.setView(m_view);
  }

  void Camera::Move(sf::Vector2f offset) {
    m_position += offset;
  }
  void Camera::Zoom(float factor) {
    m_zoom *= factor;
  }

  void Camera::SetPosition(sf::Vector2f position) {
    m_position = position;
  }
  void Camera::SetZoom(float zoom) {
    m_zoom = zoom;
  }

  sf::Vector2f Camera::GetSize() const {
    return m_view.getSize();
  }
  sf::Vector2f Camera::GetPosition() const {
    return m_position;
  }
  float Camera::GetZoom() const {
    return m_zoom;
  }
}