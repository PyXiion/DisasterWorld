#include <Disaster/Gameplay/Chunk.hpp>
#include <algorithm>
#include <cmath>

#include <Disaster/Program.hpp>
#include <easy/profiler.h>

namespace px::disaster::gameplay {
  std::array<Vector2f, kChunkSize * kChunkSize * 4> Chunk::gridVertices;
  graphics::VertexArray Chunk::gridVA;

  Chunk::Chunk(int x, int y, bool inQueue) 
    : m_x(x), 
      m_y(y), 
      // m_vertices(sf::Quads, kChunkSize * kChunkSize * 4), 
      m_wasEdited(false), 
      m_inQueue(std::make_unique<std::atomic_bool>(inQueue)) {
    m_transform.Move(m_x * kChunkSize, m_y * kChunkSize);
    m_texture = &Program::GetInstance().GetGame().GetTilemap().GetTilemapTexture();

    static auto _executeOnce = [] -> char {
      for (int i = 0; i < kChunkSize; ++i) {
        for (int j = 0; j < kChunkSize; ++j) {
          // Define position of the tile
          int index = (i + j * kChunkSize) * 4;
          gridVertices[index] = Vector2f(i, j);
          gridVertices[index + 1] = Vector2f(i + 1, j);
          gridVertices[index + 2] = Vector2f(i + 1, j + 1);
          gridVertices[index + 3] = Vector2f(i, j + 1);
        }
      }
      return 0;
    }();
  }

  int Chunk::GetX() {
    return m_x;
  }
  int Chunk::GetY() {
    return m_y;
  }

  void Chunk::Clear(TileID tile) {
    EASY_FUNCTION();
    for (int i = 0; i < kChunkSize; i++) {
      for (int j = 0; j < kChunkSize; j++) {
        m_tiles[i][j] = tile;
      }
    }
  }
  
  void Chunk::UpdateUV(IntRect area) {
    EASY_FUNCTION();
    for (int i = area.left; i < area.left + area.width; i++)
      for (int j = area.top; j < area.top + area.height; j++) {
        UpdateUV(i, j);
      }
  }
  void Chunk::UpdateUV(int x, int y) {
    int index = (x + y * kChunkSize) * 4;

    IntRect rect = Program::GetInstance().GetGame().GetTilemap().GetTileTextureRect(m_tiles[x][y]);

    m_textureCoords[index] =     Vector2f(rect.left, rect.top);
    m_textureCoords[index + 1] = Vector2f(rect.left + rect.width, rect.top);
    m_textureCoords[index + 2] = Vector2f(rect.left + rect.width, rect.top + rect.height);
    m_textureCoords[index + 3] = Vector2f(rect.left, rect.top + rect.height);
  }

  void Chunk::SetTile(int x, int y, TileID tileId) {
    m_tiles[x][y] = tileId;

    m_wasEdited = true; // indicates that the chunk must be saved when unloaded
  }
  TileID Chunk::GetTile(int x, int y) const {
    return m_tiles[x][y];
  }
  
  TileID *Chunk::GetTileRef(int x, int y) {
    return &m_tiles[x][y];
  }
  const TileID *Chunk::GetTileRef(int x, int y) const {
    return &m_tiles[x][y];
  }

  void Chunk::SetColor(int x, int y, Color color) {
    int index = (x + y * kChunkSize) * 4;

    // m_vertices[index].color = color;
    // m_vertices[index + 1].color = color;
    // m_vertices[index + 2].color = color;
    // m_vertices[index + 3].color = color;
  }

  bool Chunk::IsInQueue() const {
    return *m_inQueue;
  }
  void Chunk::SetQueueStatus(bool status) {
    EASY_FUNCTION();
    *m_inQueue = status;
  }

  void Chunk::Draw() const {
    EASY_BLOCK("Chunk::Draw");

  }

  void Chunk::Serialize(utils::MemoryStream &stream) const {
    EASY_FUNCTION();
    std::vector<std::string> globalIds;

    // Get unique IDs
    std::vector<TileID> uniqueIds;
    for (int x = 0; x < kChunkSize; x++)
      for (int y = 0; y < kChunkSize; y++)
        if (std::find(uniqueIds.begin(), uniqueIds.end(), m_tiles[x][y]) != uniqueIds.end())
          uniqueIds.push_back(m_tiles[x][y]);
    
    // Map
    Tilemap &tilemap = Program::GetInstance().GetGame().GetTilemap();
    for (int i = 0; i < uniqueIds.size(); i++) {
      TileInfo info = tilemap.GetTileInfo(uniqueIds[i]);
      globalIds.push_back(info.id);
    }

    stream << globalIds;

    for (int x = 0; x < kChunkSize; x++)
      for (int y = 0; y < kChunkSize; y++) {
        int id = std::find(uniqueIds.begin(), uniqueIds.end(), m_tiles[x][y]) - uniqueIds.begin();
        stream << id;
      }
  }

  void Chunk::Deserialize(utils::MemoryStream &stream) {
    EASY_FUNCTION();
    for (int x = 0; x < kChunkSize; x++)
      for (int y = 0; y < kChunkSize; y++) {
        stream >> m_tiles[x][y];
      }
  }
}