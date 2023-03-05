#ifndef PX_DISASTER_GAMEPLAY_CHUNK_HPP
#define PX_DISASTER_GAMEPLAY_CHUNK_HPP
#include <atomic>
#include <array>
#include <memory>
#include <cstdint>

#include <Disaster/Graphics/VertexArray.hpp>
#include <Disaster/Graphics/Texture.hpp>
#include <Disaster/Graphics/Transform.hpp>
#include <Disaster/System/Vector2.hpp>
#include <Disaster/System/Rect.hpp>
#include <Disaster/System/Color.hpp>
#include <Disaster/Utils/FastNoiseLite.h>
#include <Disaster/Utils/MemoryStream.hpp>
#include <Disaster/ITickable.hpp>

namespace px::disaster {
  typedef uint16_t TileID;
  const int kChunkSize = 256;

  namespace gameplay {
    class Chunk : public ITickable, public utils::ISerializable {
    private:
      TileID m_tiles[kChunkSize][kChunkSize];
      std::array<Vector2f, kChunkSize * kChunkSize * 4> m_textureCoords;
      Transform m_transform;

      int m_x;
      int m_y;

      bool m_wasEdited;
      std::unique_ptr<std::atomic_bool> m_inQueue;

      const graphics::Texture *m_texture;

      static std::array<Vector2f, kChunkSize * kChunkSize * 4> gridVertices;
      static graphics::VertexArray gridVA;

    public:
      Chunk(int x, int y, bool inQueue = false);

      int GetX();
      int GetY();

      void Clear(TileID tile = 0);
      /// @brief Update tile textures
      /// @param area updateable area (default: all chunk)
      void UpdateUV(IntRect area = IntRect(0, 0, kChunkSize, kChunkSize));

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

      void SetColor(int x, int y, Color color);

      bool IsInQueue() const;
      void SetQueueStatus(bool status);

      void Draw() const;

      void Serialize(utils::MemoryStream &stream) const override;
      void Deserialize(utils::MemoryStream &stream) override;
    };
  }
}

#endif // PX_DISASTER_GAMEPLAY_CHUNK_HPP