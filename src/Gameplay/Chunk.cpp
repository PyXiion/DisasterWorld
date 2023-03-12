#include <Disaster/Gameplay/Chunk.hpp>
#include <algorithm>
#include <cmath>

#include <Disaster/Program.hpp>
#include <Disaster/Graphics/Sprite.hpp>
#include <easy/profiler.h>


namespace px::disaster::gameplay {
  std::array<Vector2f, kChunkSize * kChunkSize * 4> Chunk::gridVertices;
  graphics::VertexArray Chunk::gridVA;

  Chunk::Chunk(Vector2i position) 
    : m_position(position), m_loaded(std::make_unique<std::atomic_bool>(false)) {
    m_transform.Move((position * kChunkSize).Convert<float>());
    m_texture = &Program::GetInstance().GetGame().GetTilemap().GetTilemapTexture();
  }

  Vector2i Chunk::GetPosition() const {
    return m_position;
  }

  void Chunk::Clear(TileID tile) {
    EASY_BLOCK("Chunk::Clear");
    for (int i = 0; i < kChunkSize; i++) {
      for (int j = 0; j < kChunkSize; j++) {
        m_tiles[i][j] = tile;
      }
    }
  }
  
  void Chunk::UpdateTileTextures(IntRect area) {
    EASY_BLOCK("Chunk::UpdateTileTextures");
    for (int i = area.left; i < area.left + area.width; i++)
      for (int j = area.top; j < area.top + area.height; j++) {
        UpdateTileTexture({i, j});
      }
  }
  void Chunk::UpdateTileTexture(Vector2i position) {
    EASY_BLOCK("Chunk::UpdateTileTexture");
    int index = (position.x + position.y * kChunkSize) * 4;
    
    TileID id = GetTile(position);
    IntRect rect = Program::GetInstance().GetGame().GetTilemap().GetTileTextureRect(id);

    m_textureCoords[index] =     Vector2f(rect.left, rect.top);
    m_textureCoords[index + 1] = Vector2f(rect.left + rect.width, rect.top);
    m_textureCoords[index + 2] = Vector2f(rect.left + rect.width, rect.top + rect.height);
    m_textureCoords[index + 3] = Vector2f(rect.left, rect.top + rect.height);
  }

  void Chunk::SetTile(Vector2i position, TileID tileId) {
    m_tiles[position.x][position.y] = tileId;
  }
  TileID Chunk::GetTile(Vector2i position) const {
    return m_tiles[position.x][position.y];
  }
  
  TileID *Chunk::GetTilePtr(Vector2i position) {
    return &m_tiles[position.x][position.y];
  }
  const TileID *Chunk::GetTilePtr(Vector2i position) const {
    return &m_tiles[position.x][position.y];
  }

  void Chunk::SetTileColor(Vector2i position, Color color) {
    // TODO: realise later
  }

  void Chunk::SetLoadedStatus(bool loaded) {
    *m_loaded = loaded;
  }
  bool Chunk::IsLoaded() const {
    return *m_loaded;
  }

  IntRect Chunk::GetBounds() const {
    return IntRect(m_position.x * kChunkSize, m_position.y * kChunkSize, kChunkSize, kChunkSize);
  }

  void Chunk::Draw() const {
    EASY_BLOCK("Chunk::Draw");
    // TODO: realise later

    static graphics::Sprite sprite;
    sprite.SetPosition((m_position * kChunkSize).Convert<float>());
    sprite.SetSize(kChunkSize, kChunkSize);
    sprite.Draw();
  }

  void Chunk::GenerateGridVertices() {
    EASY_BLOCK("Chunk::GenerateGridVertices");
    for (int i = 0; i < kChunkSize; ++i) {
      for (int j = 0; j < kChunkSize; ++j) {
        int index = (i + j * kChunkSize) * 4;

        gridVertices[index] = Vector2f(i, j);
        gridVertices[index + 1] = Vector2f(i + 1, j);
        gridVertices[index + 2] = Vector2f(i + 1, j + 1);
        gridVertices[index + 3] = Vector2f(i, j + 1);
      }
    }
  }

  void Chunk::Serialize(utils::MemoryStream &stream) const {
    EASY_BLOCK("Chunk::Serialize");
    // TODO: realise later
  }
  void Chunk::Deserialize(utils::MemoryStream &stream) {
    EASY_BLOCK("Chunk::Deserialize");
    // TODO: realise later
  }
}