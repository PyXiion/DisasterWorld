#ifndef PX_DISASTER_GRAPHICS_SPRITE_HPP
#define PX_DISASTER_GRAPHICS_SPRITE_HPP
#include <array>

#include <Disaster/Graphics/Transformable.hpp>
#include <Disaster/Graphics/Shader.hpp>
#include <Disaster/Graphics/Texture.hpp>

namespace px::disaster::graphics {
  class Sprite : public Transformable {
  public:
    Sprite(const Texture *texture = nullptr, Color color = Color(255, 255, 255, 255));

    void Draw() const;

    static Shader &GetShader();

  private:
    const Texture *m_texture;
    Color m_color;

    static Shader shader;
    static std::array<float, 24> vertices;
    static unsigned int vao;
    static unsigned int vbo;
  };
}

#endif // PX_DISASTER_GRAPHICS_SPRITE_HPP