#ifndef PX_DISASTER_GAMEPLAY_TILE_INFO_HPP
#define PX_DISASTER_GAMEPLAY_TILE_INFO_HPP
#include <string>

#include <Disaster/Graphics/Texture.hpp>
#include <Disaster/System/Rect.hpp>

namespace px::disaster::gameplay {
  struct TileInfo {
    std::string id;
    std::string textureId;
    
    graphics::Texture *texture = nullptr;
    IntRect tilesetRect;
  };
}

#endif // PX_DISASTER_GAMEPLAY_TILE_INFO_HPP