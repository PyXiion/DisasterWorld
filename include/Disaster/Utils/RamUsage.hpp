#ifndef PX_DISASTER_UTILS_RAM_USAGE_HPP
#define PX_DISASTER_UTILS_RAM_USAGE_HPP
#include <cstddef>

namespace px::disaster::utils {
  /// @brief The amount of memory currently being used by this process, in bytes.
  /// @return 
  extern size_t GetUsedMemory(bool resident = false);
}

#endif // PX_DISASTER_UTILS_RAM_USAGE_HPP