#include <Disaster/Graphics/VertexArray.hpp>
#include <glad/glad.h>

namespace px::disaster::graphics {
  constexpr GLint GetGLType(VertexArray::ArrayType type) {
    switch (type) {
      case VertexArray::ArrayType_STATIC: return GL_STATIC_DRAW;
      case VertexArray::ArrayType_DYNAMIC: return GL_DYNAMIC_DRAW;
      case VertexArray::ArrayType_STREAM: return GL_STREAM_DRAW;
      default: throw;
    }
  }

  VertexArray::VertexArray(ArrayType type, VAFlag flags) 
    : m_arrayType(type), m_flags(flags), m_vertexCount(0), m_VAO(0), m_VBO{0, 0, 0} {}
  VertexArray::~VertexArray() {
    if (m_VBO[0]) glDeleteBuffers(1, &m_VBO[0]);
    if (m_VBO[1]) glDeleteBuffers(1, &m_VBO[1]);
    if (m_VBO[2]) glDeleteBuffers(1, &m_VBO[2]);
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
  }

  void VertexArray::SetArrayType(ArrayType type) {
    m_arrayType = type;
  }
  void VertexArray::SetFlags(VAFlag flags) {
    m_flags = flags;
  }

  void VertexArray::SetVerticesBuffer(unsigned int buffer, size_t vertexCount) {
    if (m_VBO[0])
      glDeleteBuffers(1, &m_VBO[0]);
    m_VBO[0] = buffer;
    m_vertexCount = vertexCount;

    SetupLayouts();
  }
  std::pair<unsigned int, size_t> VertexArray::GetVerticesBuffer() const {
    return std::make_pair(m_VBO[0], m_vertexCount);
  }

  void VertexArray::SetData(size_t vSize, 
    const float *vertices, 
    const float *textureCoords, 
    const float *colors) {
    if (!m_VAO)
      glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    m_vertexCount = vSize;

    if (vertices && vSize) {
      if (!m_VBO[0]) glGenBuffers(1, &m_VBO[0]);
      glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
      glBufferData(GL_ARRAY_BUFFER, vSize * sizeof(float), vertices, GetGLType(m_arrayType));
    }
    if (textureCoords) {
      if (!m_VBO[1]) glGenBuffers(1, &m_VBO[1]);
      glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
      glBufferData(GL_ARRAY_BUFFER, vSize * sizeof(float), textureCoords, GetGLType(m_arrayType));
    }
    if (colors) {
      if (!m_VBO[2]) glGenBuffers(1, &m_VBO[2]);
      glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
      glBufferData(GL_ARRAY_BUFFER, vSize * sizeof(float), colors, GetGLType(m_arrayType));
    }
    
    SetupLayouts();
  }
  void VertexArray::SetData(const std::vector<float> *vertices, 
    const std::vector<float> *textureCoords, 
    const std::vector<float> *colors) {
    SetData((vertices) ? vertices->size() : 0, 
            (vertices) ? vertices->data() : nullptr, 
            (textureCoords) ? textureCoords->data() : nullptr,
            (colors) ? colors->data() : nullptr);
  }
  
  void VertexArray::SetupLayouts() {
    glBindVertexArray(m_VAO);
    int loc = 0;
    intptr_t stride = 0;
    intptr_t offset = 0;
    if (m_VBO[1]) stride += 2;
    if (m_VBO[2]) stride += (m_flags & VAFlag_RGBA) ? 4 : 3;

    if (m_VBO[0]) {
      glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
      glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, stride, 0);
      glEnableVertexAttribArray(loc);
      loc += 1;
      offset += 2;
    }
    if (m_VBO[1]) {
      glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
      glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
      glEnableVertexAttribArray(loc);
      loc += 1;
    }
    else if (m_flags & VAFlag_TEXTURE_COORDS) {
      glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
      glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, stride, (void*) offset);
      glEnableVertexAttribArray(loc);
      loc += 1;
      offset += 2;
    }
    if (m_VBO[2]) {
      glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
      glVertexAttribPointer(loc, (m_flags & VAFlag_RGBA) ? 4 : 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
      glEnableVertexAttribArray(loc);
      loc += 1;
    }
    else if (m_flags & VAFlag_COLORS) {
      glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
      glVertexAttribPointer(loc, (m_flags & VAFlag_RGBA) ? 4 : 3, GL_FLOAT, GL_FALSE, stride, (void*) offset);
      glEnableVertexAttribArray(loc);
      loc += 1;
      offset += (m_flags & VAFlag_RGBA) ? 4 : 3;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void VertexArray::Draw(const Shader *shader, DrawMode mode) const {
    if (!m_VBO[0] || !m_vertexCount) return;
    if (shader)
      shader->Use();
    glBindVertexArray(m_VAO);
    glDrawArrays((mode == DrawMode_TRIANGLES) ? GL_TRIANGLES : GL_QUADS, 0, m_vertexCount);
    glBindVertexArray(0);
  }
}