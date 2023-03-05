#ifndef PX_DISASTER_GAMEPLAY_WORLD_HPP
#define PX_DISASTER_GAMEPLAY_WORLD_HPP
#include <vector>
#include <queue>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>

#include <SFML/Graphics.hpp>
#include <Disaster/ITickable.hpp>
#include <Disaster/Gameplay/Chunk.hpp>
#include <Disaster/Gameplay/BaseWorldGenerator.hpp>
#include <Disaster/ThreadSafe/Queue.hpp>
#include <Disaster/Utils/FastNoiseLite.h>

namespace px::disaster::gameplay {
  class World : public ITickable {
    friend class Game;
  private:
    std::mutex m_chunksMutex;
    std::vector<std::unique_ptr<Chunk>> m_chunks;

    thread_safe::Queue<Chunk *> m_chunksQueue;
    
    std::thread m_chunkLoadThread;
    std::atomic_bool m_stopChunkLoadThread;

    IWorldGenerator *m_worldGenerator;


    void ChunkLoadThread();

  public:
    World();
    ~World();

    virtual void TickMedium(float tickDelta) override;

    /// @brief Get a chunk by coordinates, creates new if it does not exist
    /// @param x X world coordinate
    /// @param y Y world coordinate
    /// @return Pointer to a chunk
    Chunk *GetChunkUnsafe(float x, float y);

    /// @brief Get a chunk by coordinates, creates new if it does not exist
    /// @param x X world coordinate
    /// @param y Y world coordinate
    /// @param safe enable tread-safe
    /// @return Pointer to a chunk
    Chunk *GetChunk(float x, float y);

    /// @brief Request to load XY chunk
    /// @param x chunk X
    /// @param y chunk Y
    /// @return false if already loaded
    bool RequestChunkUnsafe(int x, int y);

    /// @brief Request to load XY chunk
    /// @param x chunk X
    /// @param y chunk Y
    /// @return false if already loaded
    bool RequestChunk(int x, int y);

    /// @brief Get loaded chunks (not thread-safe)
    /// @return loaded chunks
    /// @see GetChunksMutex
    std::vector<std::unique_ptr<Chunk>> &GetChunks();
    
    /// @brief Get mutex for vector of loaded chunks
    /// @return mutex
    std::mutex &GetChunksMutex();

    void Draw() const;
  };
}

#endif // PX_DISASTER_GAMEPLAY_WORLD_HPP