#include <Disaster/Script/ScriptEngine.hpp>
#include <string>

#include <Disaster/AppConsole.hpp>
#include <angelscript/scriptstdstring.h>
#include <easy/profiler.h>

namespace px::disaster::script {
  ScriptEngine::ScriptEngine() {
    EASY_BLOCK("ScriptEngine::ScriptEngine");
    m_engine = asCreateScriptEngine();

    m_engine->SetMessageCallback(asFUNCTION(ScriptEngine::MessageCallback), 0, asCALL_CDECL);

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

  void ScriptEngine::MessageCallback(const asSMessageInfo *msg, void *param) {
    static AppConsole *console = AppConsole::GetInstance();

    std::string type = "[error]";
    if (msg->type == asMSGTYPE_WARNING)
      type = "[warning]";
    else if (msg->type == asMSGTYPE_INFORMATION)
      type = "[info]";
    
    console->AddLog("%s [Scripts] %s (%d, %d) : %s", type.c_str(), msg->section, msg->row, msg->col, msg->message);
  }
}