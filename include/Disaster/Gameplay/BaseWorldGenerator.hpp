#ifndef PX_DISASTER_GAMEPLAY_BASE_WORLD_GENERATOR_HPP
#define PX_DISASTER_GAMEPLAY_BASE_WORLD_GENERATOR_HPP
#include <Disaster/Gameplay/IWorldGenerator.hpp>
#include <Disaster/Utils/FastNoiseLite.h>

namespace px::disaster::gameplay {
  class BaseWorldGenerator : public IWorldGenerator {
  private:
    FastNoiseLite m_noisePerlin;
    FastNoiseLite m_noiseSimplex;

  public:
    BaseWorldGenerator();

    void GenerateChunk(Chunk &chunk) override;
  };
}

#endif // PX_DISASTER_GAMEPLAY_BASE_WORLD_GENERATOR_HPP