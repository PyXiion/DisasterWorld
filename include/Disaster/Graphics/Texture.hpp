#ifndef PX_DISASTER_GRAPHICS_TEXTURE_HPP
#define PX_DISASTER_GRAPHICS_TEXTURE_HPP
#include <string>

#include <Disaster/System/Vector2.hpp>

namespace px::disaster::graphics {
  enum TextureFlags {
    TextureFlags_RGB = 0,
    TextureFlags_RGBA = 1 << 0
  };

  class Texture {
  public:
    Texture(TextureFlags flags = TextureFlags_RGB);
    ~Texture();
    
    void Create(int width, int height);
    void LoadFromFile(const std::string &filename);

    Vector2u GetSize() const;

    void Use() const;
    unsigned int GetHandle() const;

  private:
    unsigned int m_texture;
    Vector2u m_size;

    TextureFlags m_flags;
  };
}

#endif // PX_DISASTER_GRAPHICS_TEXTURE_HPP