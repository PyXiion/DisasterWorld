#ifndef PX_DISASTER_PROGRAM_HPP
#define PX_DISASTER_PROGRAM_HPP
#include <Disaster/Gameplay/Game.hpp>
#include <Disaster/Utils/TextureManager.hpp>
#include <Disaster/AppConsole.hpp>

namespace px::disaster {
  class Program {
    friend class gameplay::Game;
  private:
    sf::RenderWindow m_window;

    utils::TextureManager *m_textureManager;
    gameplay::Game *m_game;

    AppConsole *m_console;
    script::ScriptEngine *m_scriptEngine;

  public:
    void Main();

    sf::RenderWindow &GetWindow();
    gameplay::Game &GetGame();
    utils::TextureManager &GetTextureManager();
    script::ScriptEngine &GetScriptEngine();

    static Program &GetInstance();
  };
}

#endif // PX_DISASTER_PROGRAM_HPP