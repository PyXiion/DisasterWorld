#ifndef PX_DISASTER_PROGRAM_HPP
#define PX_DISASTER_PROGRAM_HPP
#include <Disaster/Gameplay/Game.hpp>
#include <Disaster/System/Window.hpp>
#include <Disaster/AppConsole.hpp>

namespace px::disaster {
  class Program {
    friend class gameplay::Game;
  private:
    system::Window m_window;

    gameplay::Game *m_game;

    AppConsole *m_console;
    script::ScriptEngine *m_scriptEngine;

  public:
    /// @brief The main stream of the program. Run it to start the program
    void Main();
    
    system::Window &GetWindow();
    gameplay::Game &GetGame();
    script::ScriptEngine &GetScriptEngine();

    static Program &GetInstance();
  };
}

#endif // PX_DISASTER_PROGRAM_HPP