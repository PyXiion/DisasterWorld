#include <Disaster/Utils/TextureManager.hpp>
#include <filesystem>
#include <stdexcept>

#include <Disaster/AppConsole.hpp>
#include <easy/profiler.h>

namespace fs = std::filesystem;

namespace px::disaster::utils {
  std::string TextureManager::GetIdFromFilename(std::string filename) {
    EASY_FUNCTION();
    // Get filename without extension
    fs::path path(filename);
    std::string id = path.filename();
    id = id.substr(0, id.find_last_of('.'));
    
    // Add number id if exists
    if (m_textures.find(id) != m_textures.end()) {
      int i = 1;
      while (m_textures.find(id + std::to_string(i)) != m_textures.end())
        i += 1;
      id += std::to_string(i);
    }
    
    return id;
  }

  std::string TextureManager::LoadTexture(std::string filename) {
    EASY_FUNCTION();
    // Check existance and type
    if (!fs::exists(filename))
      throw std::runtime_error("File \"" + filename +"\" does not exists.");
    if (!fs::is_regular_file(filename))
      throw std::runtime_error("\"" + filename + "\" is not regular file.");
    
    std::unique_ptr<sf::Texture> texture(new sf::Texture);
    if (!texture->loadFromFile(filename))
      throw std::runtime_error("Failed to load texture \"" + filename + "\".");
    
    std::string id = GetIdFromFilename(filename);
    m_textures[id] = std::move(texture);

    PX_LOG("Loaded \"%s\" texture", filename.c_str());

    return id;
  }

  void TextureManager::ReplaceTexture(std::string textureId, std::string filename) {
    
  }

  sf::Texture &TextureManager::GetTexture(std::string textureId) {
    return *m_textures[textureId];
  }

  bool TextureManager::TextureExists(std::string textureId) {
    return m_textures.find(textureId) != m_textures.end();
  }
}