#ifndef PX_DISASTER_GRAPHICS_TRANSFORM_HPP
#define PX_DISASTER_GRAPHICS_TRANSFORM_HPP
#include <Disaster/System/Vector2.hpp>
#include <glm/glm.hpp>

namespace px::disaster {
  class Transform {
  public:
    Transform();

    void SetPosition(Vector2f position);
    void SetPosition(float x, float y);
    Vector2f GetPosition() const;

    void Move(Vector2f offset);
    void Move(float x, float y);

    void Scale(Vector2f scale);
    void Scale(float factor);

    glm::mat4 GetMat4() const;
    const glm::mat4 *GetMat4Ptr() const;    

  private:
    glm::mat4 m_transform;
  };
}

#endif // PX_DISASTER_GRAPHICS_TRANSFORM_HPP