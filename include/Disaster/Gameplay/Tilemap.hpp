#ifndef PX_DISASTER_GAMEPLAY_TILEMAP_HPP
#define PX_DISASTER_GAMEPLAY_TILEMAP_HPP
#include <string>
#include <vector>
#include <unordered_map>

#include <Disaster/Gameplay/Chunk.hpp>
#include <Disaster/Gameplay/TileInfo.hpp>
#include <Disaster/Graphics/RenderTexture.hpp>
#include <Disaster/System/Rect.hpp>
#include <SFML/Graphics.hpp>

namespace px::disaster::gameplay {
  const int kMaxTileCount = 1024;
  const int kModTilesStartIndex = 512;
  
  class Tilemap {
  private:
    graphics::RenderTexture m_tilemapTexture;
    std::vector<TileInfo> m_tiles;

    void FitTiles();

  public:
    Tilemap();

    /// @brief Load tilemap from configuration file
    /// @param configurationFilename path to YAML configuration file
    void LoadTilemap(std::string configurationFilename);

    /// @brief Get texture coordinates for the tile
    /// @param tileId numeric tile ID
    /// @return texture coordinates
    /// @throw Can throw exception if not found
    IntRect GetTileTextureRect(TileID tileId) const;

    /// @brief Get tile information by numeric ID
    /// @param tileId numeric tile ID
    /// @return tile information
    /// @throw Can throw exception if not found
    TileInfo GetTileInfo(TileID tileId) const;

    /// @brief Get tile information by string ID
    /// @param tileId string tile ID
    /// @return tile information
    /// @throw Can throw exception if not found
    TileInfo GetTileInfo(std::string tileId) const;

    /// @brief Get tilemap texture
    /// @return tilemap texture
    const graphics::Texture &GetTilemapTexture() const;

    const std::vector<TileInfo> &GetTiles() const;
  };
}

#endif // PX_DISASTER_GAMEPLAY_TILEMAP_HPP