#ifndef PX_DISASTER_GAMEPLAY_WORLD_HPP
#define PX_DISASTER_GAMEPLAY_WORLD_HPP
#include <vector>
#include <queue>
#include <thread>
#include <memory>
#include <mutex>
#include <list>
#include <set>
#include <optional>
#include <condition_variable>

#include <SFML/Graphics.hpp>
#include <Disaster/Tickable.hpp>
#include <Disaster/Gameplay/Chunk.hpp>
#include <Disaster/Gameplay/BaseWorldGenerator.hpp>
#include <Disaster/System/Vector2.hpp>
#include <Disaster/ThreadSafe/Queue.hpp>

namespace px::disaster::gameplay {
  typedef std::shared_ptr<Chunk> ChunkPtr;
  typedef std::weak_ptr<Chunk> WeakChunkPtr;
  class World : public Tickable {
    friend class Game;
  public:
    World();
    ~World();

    virtual void TickMedium(float tickDelta) override;

    ChunkPtr GetChunkUnsafe(Vector2i position);
    ChunkPtr GetChunk(Vector2i position);

    bool RequestChunk(Vector2i position);
    bool DeleteChunk(Vector2i position);

    bool IsChunkLoadedOrQueued(Vector2i position);
    bool IsChunkQueued(Vector2i position);
    bool IsChunkLoaded(Vector2i position);

    size_t GetChunkCount();

    template<class Func>
    void IterateChunks(Func func);
    template<class Func>
    void DeleteChunksIf(Func func);

    void Draw() const;

  private:
    std::vector<ChunkPtr> m_chunks;
    std::mutex m_chunksMutex;

    thread_safe::Queue<WeakChunkPtr> m_chunkRequests;
    
    std::thread m_chunkProcessingThread;
    std::atomic_bool m_terminateThreads;

    IWorldGenerator *m_worldGenerator;

    void AppendChunk(ChunkPtr &&chunk);

    void ChunkLoadThread();
  };

  template<class Func>
  void World::IterateChunks(Func func) {
    std::lock_guard<std::mutex> lk(m_chunksMutex);
    std::for_each(m_chunks.begin(), m_chunks.end(), func);
  }
  template<class Func>
  void World::DeleteChunksIf(Func func) {
    std::lock_guard<std::mutex> lk(m_chunksMutex);
    auto begin = m_chunks.begin();
    auto end = m_chunks.end();
    auto it = std::remove_if(begin, end, func);
    m_chunks.erase(it, end);
  }
}

#endif // PX_DISASTER_GAMEPLAY_WORLD_HPP