#include <Disaster/Gameplay/Camera.hpp>
#include <Disaster/Program.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <easy/profiler.h>

namespace px::disaster::gameplay {
  Camera::Camera() 
    : m_zoom(1.0f), m_window(Program::GetInstance().GetWindow()) {
      Update();
    }
  
  void Camera::Update() {
    EASY_BLOCK("Camera::Update");
    if (m_zoom < 0.5f) m_zoom = 0.5f;

    Vector2f winSize = m_window.GetSize().Convert<float>();
    m_size = winSize / m_zoom;
    m_projection = glm::ortho<float>(0, m_size.x, 0, m_size.y, -1.0f, 1.0f);
    m_view = glm::translate(glm::mat4(1.0f), glm::vec3(-m_position.x + m_size.x / 2.0f, -m_position.y + m_size.y / 2.0f, 0.0f));
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
    return m_size;
  }
  Vector2f Camera::GetPosition() const {
    return m_position;
  }
  float Camera::GetZoom() const {
    return m_zoom;
  }

  void Camera::Apply(graphics::Shader &shader) const {
    shader.SetUniform("projection", m_projection);
    shader.SetUniform("view", m_view);
  }
}