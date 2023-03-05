#ifndef PX_DISASTER_GRAPHICS_VERTEX_ARRAY_HPP
#define PX_DISASTER_GRAPHICS_VERTEX_ARRAY_HPP
#include <vector>
#include <array>

#include <Disaster/Graphics/Shader.hpp>

namespace px::disaster::graphics {
  class VertexArray {
  public:
    enum ArrayType {
      ArrayType_STATIC,
      ArrayType_DYNAMIC,
      ArrayType_STREAM
    };
    enum VAFlag {
      VAFlag_NONE                     = 0,

      VAFlag_TEXTURE_COORDS           = 1 << 0,
      VAFlag_COLORS                   = 1 << 1,

      VAFlag_RGB                      = 0,
      VAFlag_RGBA                     = 1 << 2,
    };

  private:
    unsigned int m_VBO[3];
    unsigned int m_VAO;
    unsigned int m_vertexCount;

    ArrayType m_arrayType;
    VAFlag m_flags;

    void SetupLayouts();

  public:
    VertexArray(ArrayType type = ArrayType_STATIC, VAFlag flags = VAFlag_NONE);
    ~VertexArray();

    void SetArrayType(ArrayType type);
    void SetFlags(VAFlag flags);

    void SetVerticesBuffer(unsigned int buffer, size_t vertexCount);
    std::pair<unsigned int, size_t> GetVerticesBuffer() const;

    void SetData(size_t vSize = 0,
      const float *vertices = nullptr,  
      const float *textureCoords = nullptr, 
      const float *colors = nullptr);
    
    void SetData(const std::vector<float> *vertices, const std::vector<float> *textureCoords = nullptr, const std::vector<float> *colors = nullptr);
    
    template<int VSize, int TSize = 0, int CSize = 0>
    void SetData(const std::array<float, VSize> *vertices = nullptr,
      const std::array<float, TSize> *textureCoords = nullptr,
      const std::array<float, CSize> *colors = nullptr) {
      SetData(VSize, (vertices) ? vertices->data() : nullptr, (textureCoords) ? textureCoords->data() : nullptr, (colors) ? colors->data() : nullptr);
    }

    void Draw(const Shader *shader = nullptr) const;
  };
}

#endif // PX_DISASTER_GRAPHICS_VERTEX_ARRAY_HPP