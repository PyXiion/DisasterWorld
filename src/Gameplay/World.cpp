#include <Disaster/Gameplay/World.hpp>
#include <cmath>

#include <Disaster/AppConsole.hpp>
#include <easy/profiler.h>

namespace px::disaster::gameplay {
  World::World() : m_chunkProcessingThread(&World::ChunkLoadThread, this), m_terminateThreads(false) {
    m_worldGenerator = new BaseWorldGenerator();
  }
  World::~World() {
    if (m_chunkProcessingThread.joinable()) {
      m_terminateThreads = true;
      m_chunkProcessingThread.join();
    }
  }

  void World::TickMedium(float tickDelta) {
    
  }

  void World::ChunkLoadThread() {
    EASY_THREAD_SCOPE("Chunk loader");
    while (true) {
      if (m_terminateThreads)
        break;
      
      WeakChunkPtr ptr;
      m_chunkRequests.WaitAndPop(ptr);
      EASY_BLOCK("Generating chunk");

      if (ChunkPtr chunk = ptr.lock()) {
        if (chunk->IsLoaded()) continue;
        m_worldGenerator->GenerateChunk(*chunk);
        chunk->SetLoadedStatus(true);
      }
    }
  }

  ChunkPtr World::GetChunkUnsafe(Vector2i position) {
    auto begin = m_chunks.begin();
    auto end = m_chunks.end();
    auto chunk = std::find_if(begin, end, [&](auto &elem) {return elem->GetPosition() == position;});
    if (chunk != end)
      return *chunk;
    return nullptr;
  }
  ChunkPtr World::GetChunk(Vector2i position) {
    std::lock_guard<std::mutex> lock(m_chunksMutex);
    return GetChunkUnsafe(position);
  }

  bool World::RequestChunk(Vector2i position) {
    EASY_BLOCK("World::RequestChunk");
    if (IsChunkLoadedOrQueued(position)) return false;

    ChunkPtr chunk = std::make_shared<Chunk>(position);
    m_chunkRequests.Push(chunk);
    AppendChunk(std::move(chunk));
    return true;
  }
  bool World::DeleteChunk(Vector2i position) {
    EASY_BLOCK("World::DeleteChunk");
    std::lock_guard<std::mutex> lk(m_chunksMutex);
    auto begin = m_chunks.begin();
    auto end = m_chunks.end();
    auto it = std::find_if(begin, end, [&](auto &elem) {return elem->GetPosition() == position;});
    if (it == end)
      return false;
    m_chunks.erase(it);
    return true;
  }

  bool World::IsChunkLoadedOrQueued(Vector2i position) {
    std::lock_guard<std::mutex> lk(m_chunksMutex);
    auto begin = m_chunks.begin();
    auto end = m_chunks.end();
    return std::find_if(begin, end, [&](auto &elem) {return elem->GetPosition() == position;}) != end;
  }
  bool World::IsChunkQueued(Vector2i position) {
    std::lock_guard<std::mutex> lk(m_chunksMutex);
    auto begin = m_chunks.begin();
    auto end = m_chunks.end();
    return std::find_if(begin, end, [&](auto &elem) {return elem->GetPosition() == position && !elem->IsLoaded();}) != end;
  }
  bool World::IsChunkLoaded(Vector2i position) {
    std::lock_guard<std::mutex> lk(m_chunksMutex);
    auto begin = m_chunks.begin();
    auto end = m_chunks.end();
    return std::find_if(begin, end, [&](auto &elem) {return elem->GetPosition() == position && elem->IsLoaded();}) != end;
  }

  size_t World::GetChunkCount() {
    std::lock_guard<std::mutex> lk(m_chunksMutex);
    return m_chunks.size();
  }

  void World::AppendChunk(ChunkPtr &&chunk) {
    std::lock_guard<std::mutex> lk(m_chunksMutex);
    m_chunks.push_back(std::move(chunk));
  }

  void World::Draw() const {
    EASY_BLOCK("World::Draw");
    for (const auto &chunk : m_chunks) {
      if (chunk->IsLoaded()) chunk->Draw();
    }
  }
}