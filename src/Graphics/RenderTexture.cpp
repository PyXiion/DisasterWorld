#include <Disaster/Graphics/RenderTexture.hpp>
#include <stdexcept>

#include <glad/glad.h>

namespace px::disaster::graphics {
  RenderTexture::RenderTexture() : m_texture(TextureFlags_RGBA), m_fbo(0), m_size(0, 0), m_lastViewport(0, 0), m_active(0) {
  }
  RenderTexture::RenderTexture(int width, int height) : RenderTexture() {
    Create(width, height);
  }
  RenderTexture::~RenderTexture() {
    if (m_fbo) glDeleteFramebuffers(1, &m_fbo);
  }

  void RenderTexture::Create(int width, int height) {
    if (m_fbo) glDeleteBuffers(1, &m_fbo);
    glGenFramebuffers(1, &m_fbo);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      throw std::runtime_error("Failed to create framebuffer.");
    
    m_size = Vector2i(width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    m_texture.Create(width, height);

    glBindTexture(GL_TEXTURE_2D, m_texture.GetHandle());

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.GetHandle(), 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void RenderTexture::Begin() {
    m_active = true;
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  }
  void RenderTexture::Display() {
    if (!m_active) return;
    m_active = false;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  const Texture &RenderTexture::GetTexture() const {
    return m_texture;
  }
}