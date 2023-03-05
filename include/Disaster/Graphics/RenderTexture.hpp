#ifndef PX_DISASTER_GRAPHICS_RENDER_TEXTURE_HPP
#define PX_DISASTER_GRAPHICS_RENDER_TEXTURE_HPP
#include <Disaster/Graphics/Texture.hpp>

namespace px::disaster::graphics {
  class RenderTexture {
  public:
    /// @brief Creates empty RenderTexture object.
    RenderTexture();

    /// @brief Creates a texture with the given size.
    /// @param width texture width in pixels
    /// @param height texture height in pixels
    RenderTexture(int width, int height);

    ~RenderTexture();

    /// @brief Creates a texture with the given size.
    /// @param width texture width in pixels
    /// @param height texture height in pixels
    void Create(int width, int height);

    /// @brief Destroys texture and frees all buffers.
    void Close();

    /// @brief Called before Display() and OpenGL calls that need to draw on this texture.
    void Begin();

    /// @brief Called after Begin() or End(). Outputs what is drawn onto the texture.
    void Display();

    const Texture &GetTexture() const;

  private:
    unsigned int m_fbo;
    unsigned int m_renderedTexture;

    bool m_active;
    
    Vector2i m_size;
    Vector2i m_lastViewport;

    Texture m_texture;
  };
}

#endif // PX_DISASTER_GRAPHICS_RENDER_TEXTURE_HPP