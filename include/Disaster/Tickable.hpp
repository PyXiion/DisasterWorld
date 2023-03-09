#ifndef PX_DISASTER_TICKABLE_HPP
#define PX_DISASTER_TICKABLE_HPP

namespace px::disaster {
  class Tickable {
  public:
    /// @brief Called 20 times per second (or more often, depending on tick rate)
    /// @param deltaTick time between this tick and the last tick in seconds (tickRate * ticksSpeed)
    inline virtual void TickHigh(float deltaTick) {}

    /// @brief Called 1 time per second (or more often, depending on tick rate)
    /// @param deltaTick time between this tick and the last tick in seconds (tickRate * ticksSpeed)
    inline virtual void TickMedium(float deltaTick) {}

    /// @brief Called every 30 seconds (or more often, depending on tick rate)
    /// @param deltaTick time between this tick and the last tick in seconds (tickRate * ticksSpeed)
    inline virtual void TickSlow(float deltaTick) {}

    /// @brief Called every 2 minutes (or more often, depending on tick rate)
    /// @param deltaTick time between this tick and the last tick in seconds (tickRate * ticksSpeed)
    inline virtual void TickVerySlow(float deltaTick) {}
    
  };
}

#endif // PX_DISASTER_TICKABLE_HPP