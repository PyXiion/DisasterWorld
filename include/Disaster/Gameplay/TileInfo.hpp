#ifndef PX_DISASTER_GAMEPLAY_TILE_INFO_HPP
#define PX_DISASTER_GAMEPLAY_TILE_INFO_HPP
#include <string>

#include <SFML/Graphics.hpp>

namespace px::disaster::gameplay {
  struct TileInfo {
    std::string id;
    std::string textureId;
    
    sf::Texture *texture = nullptr;
    sf::IntRect tilemapRect;
  };
}

#endif // PX_DISASTER_GAMEPLAY_TILE_INFO_HPP