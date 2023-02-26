#ifndef PX_DISASTER_APP_CONSOLE_HPP
#define PX_DISASTER_APP_CONSOLE_HPP
#include <vector>
#include <string>
#include <mutex>
#include <utility>

#include <Disaster/Script/ScriptEngine.hpp>
#include <imgui-sfml/imgui.h>

namespace px::disaster {

  namespace gameplay {
    class Game;
  }

  class AppConsole {
    friend class gameplay::Game;
  private:
    char m_inputBuf[256];
    std::vector<std::string> m_items;
    std::vector<std::string> m_history;
    std::vector<std::string> m_commands;
    int m_historyPos;

    bool m_open;
    bool m_autoScroll;

    ImGuiTextFilter m_filter;

    std::string m_title;
    std::mutex m_mutex;

    asIScriptEngine *m_scriptEngine;

    void ConfigureEngine();

    void Exec(std::string &arg);
    void Eval(std::string &arg);
    void AddVariable(std::string &arg);
    void DeleteVariable(std::string &arg);
    void AddFunction(std::string &arg);
    void DeleteFunction(std::string &arg);
    void ListVariables();

    int TextEditCallback(ImGuiInputTextCallbackData *data);

    static AppConsole *instance;

  public:
    AppConsole();

    void AddLog(const char *fmt, ...) IM_FMTARGS(2);
    void ClearLog();

    void Log(const std::string &message);
    void Warn(const std::string &message);
    void Error(const std::string &message);
    
    void Execute(const std::string &commandLine);

    void Draw();

    // Get eval value
    void _Grab(int v);
    void _Grab(unsigned int v);
    void _Grab(bool v);
    void _Grab(float v);
    void _Grab(double v);
    void _Grab(const std::string &v);
    void _Grab();

    static AppConsole *GetInstance();
  };
}

#define PX_PRINT(...) px::disaster::AppConsole::GetInstance()->AddLog(__VA_ARGS__)
#define PX_LOG(...) px::disaster::AppConsole::GetInstance()->AddLog("[info] " __VA_ARGS__)
#define PX_WARN(...) px::disaster::AppConsole::GetInstance()->AddLog("[warning] " __VA_ARGS__)
#define PX_ERROR(...) px::disaster::AppConsole::GetInstance()->AddLog("[error] " __VA_ARGS__)

#endif // PX_DISASTER_APP_CONSOLE_HPP