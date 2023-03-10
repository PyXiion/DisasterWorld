#ifndef PX_DISASTER_SCRIPT_SCRIPT_ENGINE_HPP
#define PX_DISASTER_SCRIPT_SCRIPT_ENGINE_HPP
#include <angelscript.h>

namespace px::disaster::script {
  class ScriptEngine {
  public:
    ScriptEngine();
    ~ScriptEngine();

    asIScriptEngine *Get();

  private:
    asIScriptEngine *m_engine;

    static void MessageCallback(const asSMessageInfo *msg, void *param);
  };
}

#endif // PX_DISASTER_SCRIPT_SCRIPT_ENGINE_HPP