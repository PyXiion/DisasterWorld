#ifndef PX_DISASTER_GAMEPLAY_IWORLD_GENERATOR_HPP
#define PX_DISASTER_GAMEPLAY_IWORLD_GENERATOR_HPP

namespace px::disaster::gameplay {
  class Chunk;

  class IWorldGenerator {
  public:
    virtual void GenerateChunk(Chunk &chunk) = 0;
  };
}

#endif // PX_DISASTER_GAMEPLAY_IWORLD_GENERATOR_HPP