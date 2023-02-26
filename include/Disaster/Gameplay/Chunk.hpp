#ifndef PX_DISASTER_GAMEPLAY_CHUNK_HPP
#define PX_DISASTER_GAMEPLAY_CHUNK_HPP
#include <atomic>
#include <cstdint>

#include <SFML/Graphics.hpp>
#include <Disaster/Utils/FastNoiseLite.h>
#include <Disaster/Utils/MemoryStream.hpp>
#include <Disaster/ITickable.hpp>

namespace px::disaster {
  typedef uint16_t TileID;
  const int kChunkSize = 256;

  namespace gameplay {
    class Chunk : public sf::Drawable, public ITickable, public utils::ISerializable {
    private:
      TileID m_tiles[kChunkSize][kChunkSize];
      sf::VertexArray m_vertices;
      sf::Transform m_transform;

      int m_x;
      int m_y;

      bool m_wasEdited;
      std::unique_ptr<std::atomic_bool> m_inQueue;

      const sf::Texture *m_texture;

    public:
      Chunk(int x, int y, bool inQueue = false);

      void GenerateVertices();

      int GetX();
      int GetY();

      void Clear(TileID tile = 0);
      /// @brief Update tile textures
      /// @param area updateable area (default: all chunk)
      void UpdateUV(sf::IntRect area = sf::IntRect(0, 0, kChunkSize, kChunkSize));

      /// @brief Update tile texture
      /// @param x X coordinate inside the chunk (from 0 to kChunkSize)
      /// @param y 
      void UpdateUV(int x, int y);

      /// @brief Set tile
      /// @param x X coordinate inside the chunk (from 0 to kChunkSize)
      /// @param y
      /// @param tileId
      void SetTile(int x, int y, TileID tileId);

      /// @brief Get tile
      /// @param x X coordinate inside the chunk (from 0 to kChunkSize)
      /// @param y 
      /// @return tile ID
      TileID GetTile(int x, int y) const;

      /// @brief Get pointer to tile ID
      /// @param x X coordinate inside the chunk (from 0 to kChunkSize)
      /// @param y 
      /// @return pointer to tile ID
      TileID *GetTileRef(int x, int y);

      /// @brief Get pointer to tile ID
      /// @param x X coordinate inside the chunk (from 0 to kChunkSize)
      /// @param y 
      /// @return pointer to tile ID
      const TileID *GetTileRef(int x, int y) const;

      void SetColor(int x, int y, sf::Color color);

      bool IsInQueue() const;
      void SetQueueStatus(bool status);

      virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

      void Serialize(utils::MemoryStream &stream) const override;
      void Deserialize(utils::MemoryStream &stream) override;
    };
  }
}

#endif // PX_DISASTER_GAMEPLAY_CHUNK_HPP