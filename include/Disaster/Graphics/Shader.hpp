#ifndef PX_DISASTER_GRAPHICS_SHADER_HPP
#define PX_DISASTER_GRAPHICS_SHADER_HPP
#include <string>

#include <Disaster/Graphics/Transform.hpp>
#include <Disaster/System/Vector2.hpp>
#include <Disaster/System/Color.hpp>

namespace px::disaster::graphics {
  class Shader {
  private:
    unsigned int m_shaderProgram;

    static std::string LoadFile(const std::string &filename);
    static unsigned int CreateShader(const std::string &code, unsigned int type);

    int GetUniform(const std::string &id) const;

  public:
    Shader();
    ~Shader();

    void LoadFromFile(const std::string &vertexShader, const std::string &fragmentShader);
    void LoadFromMemory(const std::string &vertexShader, const std::string &fragmentShader);

    void Use() const;

    void SetUniform(const std::string &id, float value);
    void SetUniform(const std::string &id, Vector2f value);
    // void SetUniform(const std::string &id, Vector3f value);
    void SetUniform(const std::string &id, Color value);
    void SetUniform(const std::string &id, const Transform &value);
    void SetUniform(const std::string &id, const glm::mat4 &value);
  };
}

#endif // PX_DISASTER_GRAPHICS_SHADER_HPP