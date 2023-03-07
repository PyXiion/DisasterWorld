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
    
    void SetSize(Vector2f size);
    void SetSize(float x, float y);
    Vector2f GetSize() const;

    void Scale(Vector2f scale);
    void Scale(float factor);

    glm::mat4 GetMat4() const;

  private:
    Vector2f m_position;
    Vector2f m_size;
  };
}

#endif // PX_DISASTER_GRAPHICS_TRANSFORM_HPP