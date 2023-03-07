#include <Disaster/Script/ScriptEngine.hpp>
#include <angelscript/scriptstdstring.h>
#include <easy/profiler.h>

namespace px::disaster::script {
  ScriptEngine::ScriptEngine() {
    EASY_BLOCK("ScriptEngine::ScriptEngine");
    m_engine = asCreateScriptEngine();

    int r;
    // r = m_engine->SetMessageCallback()

    RegisterStdString(m_engine);
  }
  ScriptEngine::~ScriptEngine() {
    EASY_BLOCK("ScriptEngine::~ScriptEngine");
    m_engine->ShutDownAndRelease();
  }

  asIScriptEngine *ScriptEngine::Get() {
    return m_engine;
  }
}