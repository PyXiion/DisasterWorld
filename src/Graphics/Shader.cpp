#include <Disaster/Graphics/Shader.hpp>
#include <fstream>
#include <sstream>
#include <exception>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace px::disaster::graphics {
  Shader::Shader() : m_shaderProgram(0) {}
  Shader::~Shader() {
    if (m_shaderProgram) glDeleteProgram(m_shaderProgram);
  }

  std::string Shader::LoadFile(const std::string &filename) {
    std::stringstream ss;
    std::ifstream ifs(filename);
    ss << ifs.rdbuf();
    return ss.str();
  }
  unsigned int Shader::CreateShader(const std::string &code, unsigned int type) {
    unsigned int shader;
    shader = glCreateShader(type);
    const char *data = code.c_str();
    glShaderSource(shader, 1, &data, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
      throw std::runtime_error("Failed to compile shader: " + std::string(infoLog));
    }
    return shader;
  }

  void Shader::LoadFromFile(const std::string &vertexShader, const std::string &fragmentShader) {
    LoadFromMemory(LoadFile(vertexShader), LoadFile(fragmentShader));
  }
  void Shader::LoadFromMemory(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int vertex = CreateShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fragment = CreateShader(fragmentShader, GL_FRAGMENT_SHADER);

    m_shaderProgram = glCreateProgram();

    glAttachShader(m_shaderProgram, vertex);
    glAttachShader(m_shaderProgram, fragment);
    glLinkProgram(m_shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(m_shaderProgram, sizeof(infoLog), nullptr, infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  void Shader::Use() const {
    glUseProgram(m_shaderProgram);
  }

  int Shader::GetUniform(const std::string &id) const {
    return glGetUniformLocation(m_shaderProgram, id.c_str());
  }

  void Shader::SetUniform(const std::string &id, float value) {
    int uniform = GetUniform(id);
    glUniform1f(uniform, value);
  }
  void Shader::SetUniform(const std::string &id, Vector2f value) {
    int uniform = GetUniform(id);
    glUniform2f(uniform, value.x, value.y);
  }
  // void Shader::SetUniform(const std::string &id, Vector3f value) {
  //   int uniform = GetUniform(id);
  //   glUniform3f(uniform, value.x, value.y, value.z);
  // }
  void Shader::SetUniform(const std::string &id, Color value) {
    int uniform = GetUniform(id);
    glUniform4f(uniform, value.r / 255.0f, value.g / 255.0f, value.b / 255.0f, value.a / 255.0f);
  }
  void Shader::SetUniform(const std::string &id, const Transform &value) {
    int uniform = GetUniform(id);
    glUniformMatrix3fv(uniform, 1, GL_FALSE, glm::value_ptr(*value.GetMat4Ptr()));
  }
  void Shader::SetUniform(const std::string &id, const glm::mat4 &value) {
    int uniform = GetUniform(id);
    glUniformMatrix3fv(uniform, 1, GL_FALSE, glm::value_ptr(value));
  }
}