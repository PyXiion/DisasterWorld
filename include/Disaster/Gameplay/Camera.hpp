#ifndef PX_DISASTER_GAMEPLAY_CAMERA_HPP
#define PX_DISASTER_GAMEPLAY_CAMERA_HPP
#include <Disaster/System/Vector2.hpp>
#include <glm/glm.hpp>

namespace px::disaster::gameplay {
  class Camera {
  private:
    glm::mat4 m_projection;
    glm::mat4 m_view;

    Vector2f m_position;
    float m_zoom;

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
  };
}

#endif // PX_DISASTER_GAMEPLAY_CAMERA_HPP