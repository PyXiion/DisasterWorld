#include <Disaster/Gameplay/BaseWorldGenerator.hpp>
#include <Disaster/Gameplay/Chunk.hpp>
#include <easy/profiler.h>

namespace px::disaster::gameplay {
  BaseWorldGenerator::BaseWorldGenerator() {
    m_noisePerlin.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_noiseSimplex.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
  }

  void BaseWorldGenerator::GenerateChunk(Chunk &chunk) {
    EASY_FUNCTION();
    for (int x = 0; x < kChunkSize; x++) {
      for (int y = 0; y < kChunkSize; y++) {
        float height = m_noisePerlin.GetNoise<float>(chunk.GetX() * kChunkSize + x, chunk.GetY() * kChunkSize + y);

        if (height >= 0) chunk.SetTile(x, y, 0);
        else chunk.SetTile(x, y, 1);
      }
    }
  }
}