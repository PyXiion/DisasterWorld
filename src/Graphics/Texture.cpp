#include <Disaster/Graphics/Texture.hpp>
#include <stdexcept>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace px::disaster::graphics {
  Texture::Texture(TextureFlags flags) : m_flags(flags), m_texture(0) {}
  Texture::~Texture() {
    if (m_texture) glDeleteTextures(1, &m_texture);
  }

  void Texture::Create(int width, int height) {
    if (!m_texture)
      glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    m_size = Vector2u(width, height);

    GLint format = (m_flags & TextureFlags_RGBA) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
  }
  void Texture::LoadFromFile(const std::string &filename) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    GLenum channels = (nrChannels == STBI_rgb) ? GL_RGB : GL_RGBA;

    if (!data)
      throw std::runtime_error("Failed to load texture.");

    m_size = Vector2u(width, height);

    if (!m_texture)
      glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // Set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, channels, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
  }

  Vector2u Texture::GetSize() const {
    return m_size;
  }
  void Texture::Use() const {
    glBindTexture(GL_TEXTURE_2D, m_texture);
  }
  unsigned int Texture::GetHandle() const {
    return m_texture;
  }

  size_t Texture::SizeOfPixel() const {
    return ((m_flags & TextureFlags_RGBA) ? 4 : 3) * sizeof(uint8_t);
  }
}