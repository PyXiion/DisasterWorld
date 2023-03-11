#include <Disaster/Gameplay/BaseWorldGenerator.hpp>
#include <Disaster/Gameplay/Chunk.hpp>
#include <easy/profiler.h>

namespace px::disaster::gameplay {
  BaseWorldGenerator::BaseWorldGenerator() {
    m_noisePerlin.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_noiseSimplex.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
  }

  void BaseWorldGenerator::GenerateChunk(Chunk &chunk) {
    EASY_BLOCK("BaseWorldGenerator::GenerateChunk");
    for (int x = 0; x < kChunkSize; x++) {
      for (int y = 0; y < kChunkSize; y++) {
        Vector2i position(x, y);
        Vector2i worldPos = chunk.GetPosition() * kChunkSize + position;
        float height = m_noisePerlin.GetNoise<float>(worldPos.x, worldPos.y);

        if (height >= 0) chunk.SetTile(position, 0);
        else chunk.SetTile(position, 1);
      }
    }
  }
}