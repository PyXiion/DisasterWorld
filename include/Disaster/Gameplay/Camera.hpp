#ifndef PX_DISASTER_GAMEPLAY_CAMERA_HPP
#define PX_DISASTER_GAMEPLAY_CAMERA_HPP
#include <Disaster/Graphics/Shader.hpp>
#include <Disaster/System/Window.hpp>
#include <Disaster/System/Vector2.hpp>
#include <Disaster/System/Rect.hpp>
#include <glm/glm.hpp>

namespace px::disaster::gameplay {
  class Camera {
  public:
    Camera();

    void Update();

    void Move(Vector2f offset);
    void Zoom(float factor);

    void SetPosition(Vector2f position);
    void SetZoom(float zoom);

    Vector2f GetSize() const;
    Vector2f GetPosition() const;
    float GetZoom() const;

    void Apply(graphics::Shader &shader) const;

    FloatRect GetViewBounds() const;
    bool IsInView(Vector2f point) const;
    bool IsInView(FloatRect rect) const;

  private:
    glm::mat4 m_projection;
    glm::mat4 m_view;

    Vector2f m_position;
    Vector2f m_size;
    float m_zoom;

    system::Window &m_window;
  };
}

#endif // PX_DISASTER_GAMEPLAY_CAMERA_HPP