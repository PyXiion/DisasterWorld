#include <Disaster/Graphics/Sprite.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

namespace px::disaster::graphics {
  graphics::Shader *Sprite::shader;
  unsigned int Sprite::vao = 0;
  unsigned int Sprite::vbo = 0;
  std::array<float, 24> Sprite::vertices = {
    // pos      // tex
    0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
  };

  Sprite::Sprite(Texture *texture, Color color) : m_texture(texture), m_color(color) {
    if (!shader && !vao && !vbo) {
      shader = new Shader();
      shader->LoadFromFile("data/shaders/sprite.vs", "data/shaders/sprite.fs");

      glGenVertexArrays(1, &vao);
      glGenBuffers(1, &vbo);

      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(), GL_STATIC_DRAW);

      glBindVertexArray(vao);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }
  }

  void Sprite::Draw() const {
    shader->Use();
    glm::mat4 model = GetTransform().GetMat4();
    Vector2f size = m_texture->GetSize().Convert<float>();
    model = glm::scale(model, glm::vec3(size.x, size.y, 0.0f));
    shader->SetUniform("model", model);
    shader->SetUniform("spriteColor", m_color);

    glActiveTexture(GL_TEXTURE0);
    m_texture->Use();

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
  }
}