#ifndef PX_DISASTER_UTILS_TEXTURE_MANAGER_HPP
#define PX_DISASTER_UTILS_TEXTURE_MANAGER_HPP
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

#include <Disaster/Graphics/Texture.hpp>

namespace px::disaster::utils {
  class TextureManager {
  public:
    /// @brief Loads the texture and returns its ID if successful
    /// @param filename path to file
    /// @throw File does not exists or cannot be loaded
    /// @return Texture ID
    std::string LoadTexture(std::string filename);

    /// @brief Replaces the texture to another
    /// @param textureId replaceable texture
    /// @param filename path to replacement texture
    void ReplaceTexture(std::string textureId, std::string filename);

    /// @brief Returns reference to Texture
    /// @param id texture ID
    graphics::Texture &GetTexture(std::string textureId);

    /// @brief Check is a texture exists
    /// @param id texture ID
    /// @return True if exists
    bool TextureExists(std::string textureId);

    std::vector<std::string> GetTextures() const;

  private:
    std::unordered_map<std::string, std::unique_ptr<graphics::Texture>> m_textures;

    std::string GetIdFromFilename(std::string filename);
  };
}

#endif // PX_DISASTER_UTILS_TEXTURE_MANAGER_HPP