#ifndef PX_DISASTER_UPDATABLE_HPP
#define PX_DISASTER_UPDATABLE_HPP

namespace px::disaster {
  class IUpdateable {
  public:
    /// @brief Called every frame
    /// @param deltaTime time between calls
    inline virtual void Update(float deltaTime) {}
  };
}

#endif // PX_DISASTER_UPDATABLE_HPP