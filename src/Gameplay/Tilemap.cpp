#include <Disaster/Gameplay/Tilemap.hpp>

#include <algorithm>

#include <Disaster/Graphics/Sprite.hpp>
#include <Disaster/AppConsole.hpp>
#include <yaml-cpp/yaml.h>
#include <easy/profiler.h>

namespace px::disaster::gameplay {
  Tilemap::Tilemap(utils::TextureManager &textureManager) : m_textureManager(textureManager) {
  }
  
  void Tilemap::FitTiles() {
    EASY_FUNCTION();
    // Sort textures by area (largest first)
    std::sort(m_tiles.begin(), m_tiles.end(), [](const TileInfo &a, const TileInfo &b) {
      return a.texture->GetSize().x * a.texture->GetSize().y > b.texture->GetSize().x * b.texture->GetSize().y;
    });

    // Determine the size of the resulting texture
    Vector2u textureSize(128, 128);
    
    std::vector<IntRect> textureRects; 
    textureRects.reserve(m_tiles.size());
    std::vector<Vector2u> freeSpaces;
    freeSpaces.emplace_back(0, 0);
    for (TileInfo &tileInfo : m_tiles) {
      IntRect textureRect;
      bool fits = true;
      do {
        // If does not fit
        if (!fits) 
          textureSize *= 2u;
        // Iterate all free spaces and check placeability
        for (auto it = freeSpaces.begin(); it != freeSpaces.end(); ++it) {
          fits = true;
          textureRect = IntRect(it->x, it->y, tileInfo.texture->GetSize().x, tileInfo.texture->GetSize().y);

          // Texture does not fit within the boundaries
          if (textureRect.left + textureRect.width >= textureSize.x ||
              textureRect.top + textureRect.height >= textureSize.y) {
            fits = false;
            continue;
          }
          // Texture overlaps with another
          for (const auto &otherRect : textureRects) {
            if (textureRect.Intersects(otherRect)) {
              fits = false;
              break;
            }
          }
          if (fits) {
            it = freeSpaces.erase(it);
            break;
          }
        }
      } while (!fits);

      tileInfo.tilesetRect = textureRect;
      textureRects.push_back(textureRect);
      freeSpaces.emplace_back(textureRect.left + textureRect.width, textureRect.top);
      freeSpaces.emplace_back(textureRect.left, textureRect.top + textureRect.height);
    }

    // Draw
    m_tilemapTexture.Create(textureSize.x, textureSize.y);
    m_tilemapTexture.Begin();
    for (size_t i = 0; i < m_tiles.size(); i++) {
      graphics::Sprite sprite(m_tiles[i].texture);
      sprite.SetPosition(textureRects[i].left, textureRects[i].top);
    }

    m_tilemapTexture.Display();
    PX_LOG("Tilemap fitted");
  }

  void Tilemap::LoadTilemap(std::string configurationFilename) {
    EASY_FUNCTION();
    PX_LOG("Loading tilemap \"%s\"", configurationFilename.c_str());
    YAML::Node config = YAML::LoadFile(configurationFilename);
    // Load config
    if (config["tiles"]) {
      YAML::Node tiles = config["tiles"];

      for (YAML::const_iterator it = tiles.begin(); it != tiles.end(); ++it) {
        TileInfo info;
        info.id = it->first.as<std::string>();
        info.textureId = it->second.as<std::string>();

        if (!m_textureManager.TextureExists(info.textureId))
          throw std::runtime_error("Texture \"" + info.textureId + "\" does not exists.");
        info.texture = &m_textureManager.GetTexture(info.textureId);

        m_tiles.push_back(info);
        PX_LOG("Loaded %s tile (texture ID: %s)", info.id.c_str(), info.textureId.c_str());
      }
    }

    FitTiles();
  }

  IntRect Tilemap::GetTileTextureRect(TileID tileId) const {
    return m_tiles[tileId].tilesetRect;
  }
  TileInfo Tilemap::GetTileInfo(TileID tileId) const {
    if (tileId >= m_tiles.size())
      throw std::out_of_range("Tile ID out of range");
    return m_tiles[tileId];
  }
  TileInfo Tilemap::GetTileInfo(std::string tileId) const {
    for (const auto &tileInfo : m_tiles) {
      if (tileInfo.id == tileId)
        return tileInfo;
    }
    throw std::out_of_range("Tile ID out of range");
  }
  const graphics::Texture &Tilemap::GetTilemapTexture() const {
    return m_tilemapTexture.GetTexture();
  }
  const std::vector<TileInfo> &Tilemap::GetTiles() const {
    return m_tiles;
  }
}