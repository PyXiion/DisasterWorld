#ifndef PX_DISASTER_GRAPHICS_TRANSFORMABLE_HPP
#define PX_DISASTER_GRAPHICS_TRANSFORMABLE_HPP
#include <Disaster/Graphics/Transform.hpp>

namespace px::disaster {
  class Transformable {
  public:
    void Move(Vector2f offset);
    void Move(float x, float y);
    void SetPosition(Vector2f position);
    void SetPosition(float x, float y);
    Vector2f GetPosition() const;

    void Scale(float factor);
    void SetSize(Vector2f size);
    void SetSize(float x, float y);
    Vector2f GetSize() const;

  protected:
    Transform &GetTransform();
    const Transform &GetTransform() const;

  private:
    Transform m_transform;
  };
}

#endif // PX_DISASTER_GRAPHICS_TRANSFORMABLE_HPP