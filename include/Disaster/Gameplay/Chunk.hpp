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
#include <Disaster/Tickable.hpp>

namespace px::disaster {
  typedef uint16_t TileID;
  const int kChunkSize = 256;

  namespace gameplay {
    class Chunk : public Tickable, public utils::ISerializable {
    public:
      Chunk(Vector2i position);

      Vector2i GetPosition() const;

      void UpdateTileTextures(IntRect area = IntRect(0, 0, kChunkSize, kChunkSize));
      void UpdateTileTexture(Vector2i position);

      void SetTile(Vector2i position, TileID tileId);

      TileID GetTile(Vector2i position) const;
      TileID *GetTilePtr(Vector2i position);
      const TileID *GetTilePtr(Vector2i position) const;

      void SetTileColor(Vector2i position, Color color);

      void SetLoadedStatus(bool loaded);
      bool IsLoaded() const;

      void Clear(TileID tile = 0);

      void Draw() const;

      static void GenerateGridVertices();

      void Serialize(utils::MemoryStream &stream) const override;
      void Deserialize(utils::MemoryStream &stream) override;
      
    private:
      TileID m_tiles[kChunkSize][kChunkSize];
      std::array<Vector2f, kChunkSize * kChunkSize * 4> m_textureCoords;
      Transform m_transform;
      Vector2i m_position;

      std::unique_ptr<std::atomic_bool> m_loaded;

      const graphics::Texture *m_texture;

      static std::array<Vector2f, kChunkSize * kChunkSize * 4> gridVertices;
      static graphics::VertexArray gridVA;
    };
  }
}

#endif // PX_DISASTER_GAMEPLAY_CHUNK_HPP