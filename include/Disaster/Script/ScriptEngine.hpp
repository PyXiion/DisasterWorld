#ifndef PX_DISASTER_SCRIPT_SCRIPT_ENGINE_HPP
#define PX_DISASTER_SCRIPT_SCRIPT_ENGINE_HPP
#include <angelscript.h>

namespace px::disaster::script {
  class ScriptEngine {
  private:
    asIScriptEngine *m_engine;

  public:
    ScriptEngine();
    ~ScriptEngine();

    asIScriptEngine *Get();
  };
}

#endif // PX_DISASTER_SCRIPT_SCRIPT_ENGINE_HPP