#include <Disaster/Gameplay/World.hpp>
#include <cmath>

#include <Disaster/AppConsole.hpp>
#include <easy/profiler.h>

namespace px::disaster::gameplay {
  World::World() : m_chunkLoadThread(&World::ChunkLoadThread, this), m_stopChunkLoadThread(false) {
    m_worldGenerator = new BaseWorldGenerator();
  }
  World::~World() {
    if (m_chunkLoadThread.joinable()) {
      m_stopChunkLoadThread = true;
      m_chunkLoadThread.join();
    }
  }

  void World::TickMedium(float tickDelta) {
    
  }

  void World::ChunkLoadThread() {
    EASY_THREAD_SCOPE("Chunk loader");
    while (true) {
      if (m_stopChunkLoadThread)
        break;
    
      Chunk *chunk;

      m_chunksQueue.WaitAndPop(chunk);

      EASY_BLOCK("Loading chunk", profiler::colors::Orange500);
      // chunk->GenerateVertices();
      m_worldGenerator->GenerateChunk(*chunk);
      chunk->UpdateUV();
      chunk->SetQueueStatus(false);
      EASY_END_BLOCK
    }
  }

  Chunk *World::GetChunkUnsafe(float x, float y) {
    EASY_FUNCTION();
    int ix = static_cast<int>(std::floor(x / kChunkSize));
    int iy = static_cast<int>(std::floor(y / kChunkSize));
    for (auto& chunk : m_chunks) {
      if (chunk->GetX() == ix && chunk->GetY() == iy) {
        return chunk.get();
      }
    }
    return nullptr;
  }
  // Thread-safe
  Chunk *World::GetChunk(float x, float y) {
    EASY_FUNCTION();
    std::lock_guard<std::mutex> lock(m_chunksMutex);
    return GetChunkUnsafe(x, y);
  }

  bool World::RequestChunkUnsafe(int x, int y) {
    EASY_BLOCK("World::RequestChunk");
    for (auto& chunk : m_chunks) {
      if (chunk->GetX() == x && chunk->GetY() == y) {
        return false;
      }
    }
    PX_LOG("Chunk %d %d requested", x, y);
    // PX_LOG("Requested chunk %d %d", x, y);
    m_chunks.push_back(std::make_unique<Chunk>(x, y, true));
    m_chunksQueue.Push(m_chunks.back().get());
    return true;
  }

  bool World::RequestChunk(int x, int y) {
    std::lock_guard<std::mutex> lk_a(m_chunksMutex);
    return RequestChunkUnsafe(x, y);
  }

  std::vector<std::unique_ptr<Chunk>> &World::GetChunks() {
    return m_chunks;
  }
  std::mutex &World::GetChunksMutex() {
    return m_chunksMutex;
  }


  void World::Draw() const {
    EASY_BLOCK("World::Draw");
    for (const auto &chunk : m_chunks) {
      if (chunk->IsInQueue()) continue;
      chunk->Draw();
    }
  }
}