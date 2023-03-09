#ifndef PX_DISASTER_SCRIPT_BINDINGS_HPP
#define PX_DISASTER_SCRIPT_BINDINGS_HPP
#include <Disaster/Script/ScriptEngine.hpp>

namespace px::disaster::script {
  /// @brief Registers all Disaster classes in AngelScript
  /// @param engine script engine
  extern void ConfigureEngine(ScriptEngine &engine);
}

#endif // PX_DISASTER_SCRIPT_BINDINGS_HPP