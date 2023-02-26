#include <Disaster/Script/ScriptEngine.hpp>
#include <angelscript/scriptstdstring.h>

namespace px::disaster::script {
  ScriptEngine::ScriptEngine() {
    m_engine = asCreateScriptEngine();

    int r;
    // r = m_engine->SetMessageCallback()

    RegisterStdString(m_engine);
  }
  ScriptEngine::~ScriptEngine() {
    m_engine->ShutDownAndRelease();
  }

  asIScriptEngine *ScriptEngine::Get() {
    return m_engine;
  }
}