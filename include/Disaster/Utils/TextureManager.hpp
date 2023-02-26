#ifndef PX_DISASTER_UTILS_TEXTURE_MANAGER_HPP
#define PX_DISASTER_UTILS_TEXTURE_MANAGER_HPP
#include <string>
#include <unordered_map>
#include <memory>

#include <SFML/Graphics.hpp>

namespace px::disaster::utils {
  class TextureManager {
  private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textures;

    std::string GetIdFromFilename(std::string filename);

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
    sf::Texture &GetTexture(std::string textureId);

    /// @brief Check is a texture exists
    /// @param id texture ID
    /// @return True if exists
    bool TextureExists(std::string textureId);
  };
}

#endif // PX_DISASTER_UTILS_TEXTURE_MANAGER_HPP