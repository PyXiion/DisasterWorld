#ifndef PX_DISASTER_APP_CONSOLE_HPP
#define PX_DISASTER_APP_CONSOLE_HPP
#include <vector>
#include <string>
#include <mutex>
#include <utility>

#include <Disaster/Script/ScriptEngine.hpp>
#include <imgui/imgui.h>

namespace px::disaster {

  namespace gameplay {
    class Game;
  }

  class AppConsole {
    friend class gameplay::Game;
  public:
    AppConsole();

    /// @brief Formats the message and prints it to the console and cout.
    void AddLog(const char *fmt, ...) IM_FMTARGS(2);

    /// @brief Clears the console.
    void ClearLog();

    /// @brief Prints a message from the string.
    /// @param message message
    void Log(const std::string &message);

    /// @brief Prints a string in "[warning] message" format. It will be highlighted in yellow in the console.
    /// @param message message
    void Warn(const std::string &message);

    /// @brief Prints a string in "[error] message" format. It will be highlighted in red in the console.
    /// @param message message
    void Error(const std::string &message);
    
    /// @brief Executes the console command.
    /// @param commandLine command line
    void Execute(const std::string &commandLine);

    /// @brief Draws the console itself in the UI using ImGui
    void Draw();

    // Allows get the value from the eval
    void _Grab(int v);
    void _Grab(unsigned int v);
    void _Grab(bool v);
    void _Grab(float v);
    void _Grab(double v);
    void _Grab(const std::string &v);
    void _Grab();

    /// @brief Returns an instance of the last console already created or nullptr.
    static AppConsole *GetInstance();

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

    /// @brief configures AngelScript engine
    void ConfigureEngine();

    /// @brief Executes AngelScript code
    /// @param arg code
    void Exec(std::string &arg);

    /// @brief Executes AngelScript expresion and prints the value
    /// @param arg expression
    void Eval(std::string &arg);

    void AddVariable(std::string &arg);
    void DeleteVariable(std::string &arg);
    void AddFunction(std::string &arg);
    void DeleteFunction(std::string &arg);
    void ListVariables();
    void CheckMemoryUsage();

    /// @brief Manages console input history and auto-complete commands on Tab
    int TextEditCallback(ImGuiInputTextCallbackData *data);

    static AppConsole *instance;
  };
}

#define PX_PRINT(...) px::disaster::AppConsole::GetInstance()->AddLog(__VA_ARGS__)
#define PX_LOG(...) px::disaster::AppConsole::GetInstance()->AddLog("[info] " __VA_ARGS__)
#define PX_WARN(...) px::disaster::AppConsole::GetInstance()->AddLog("[warning] " __VA_ARGS__)
#define PX_ERROR(...) px::disaster::AppConsole::GetInstance()->AddLog("[error] " __VA_ARGS__)

#endif // PX_DISASTER_APP_CONSOLE_HPP