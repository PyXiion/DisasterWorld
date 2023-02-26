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
      m_chunkQueueCond.notify_one();
      m_stopChunkLoadThread = true;
      m_chunkLoadThread.join();
    }
  }

  void World::TickMedium(float tickDelta) {
    
  }

  void World::ChunkLoadThread() {
    EASY_THREAD_SCOPE("Chunk loader");
    while (true) {
      std::unique_lock<std::mutex> lk_a(m_chunksQueueMutex);
      m_chunkQueueCond.wait(lk_a, [this]{return !m_chunkQueue.empty();});

      if (m_stopChunkLoadThread)
        break;
      
      std::queue<Chunk *> chunks;
      std::swap(chunks, m_chunkQueue);
      lk_a.unlock();

      while (!chunks.empty()) {
        EASY_BLOCK("Loading chunk", profiler::colors::Orange500);
        Chunk *chunk = chunks.front();
        chunks.pop();

        chunk->GenerateVertices();
        m_worldGenerator->GenerateChunk(*chunk);
        chunk->UpdateUV();
        chunk->SetQueueStatus(false);
        EASY_END_BLOCK
      }
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
    EASY_FUNCTION();
    for (auto& chunk : m_chunks) {
      if (chunk->GetX() == x && chunk->GetY() == y) {
        return false;
      }
    }
    PX_LOG("Chunk %d %d requested", x, y);
    // PX_LOG("Requested chunk %d %d", x, y);
    m_chunks.push_back(std::make_unique<Chunk>(x, y, true));
    m_chunkQueue.push(m_chunks.back().get());
    m_chunkQueueCond.notify_one();
    return true;
  }

  bool World::RequestChunk(int x, int y) {
    EASY_FUNCTION();
    std::lock(m_chunksQueueMutex, m_chunksMutex);
    std::lock_guard<std::mutex> lk_a(m_chunksMutex, std::adopt_lock);
    std::lock_guard<std::mutex> lk_b(m_chunksQueueMutex, std::adopt_lock);
    return RequestChunkUnsafe(x, y);
  }

  std::vector<std::unique_ptr<Chunk>> &World::GetChunks() {
    return m_chunks;
  }
  std::mutex &World::GetChunksMutex() {
    return m_chunksMutex;
  }


  void World::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    EASY_BLOCK("World::draw");
    for (const auto &chunk : m_chunks) {
      if (chunk->IsInQueue()) continue;
      target.draw(*chunk, states);
    }
  }
}