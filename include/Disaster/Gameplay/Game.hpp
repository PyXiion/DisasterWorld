#ifndef PX_DISASTER_GAME_HPP
#define PX_DISASTER_GAME_HPP
#include <SFML/Graphics.hpp>
#include <Disaster/ITickable.hpp>
#include <Disaster/Gameplay/World.hpp>
#include <Disaster/Gameplay/Camera.hpp>
#include <Disaster/Gameplay/GameSettings.hpp>
#include <Disaster/Gameplay/Tilemap.hpp>
#include <Disaster/Utils/TextureManager.hpp>

namespace px::disaster {
  class Program;

  namespace gameplay {
    class Game : private ITickable {
    private:
      Program &m_program;
      sf::RenderWindow &m_window;
      GameSettings m_settings;

      float m_deltaTime;

      float m_tickSpeed = 1.0f;
      int m_tickMask = 0;
      std::array<float, 4> m_elapsedTimes;
      std::array<float, 4> m_tickRates;
      std::array<void (Game::*)(float), 4> m_tickFunctions;

      World *m_world;
      Camera *m_camera;

      utils::TextureManager &m_textureManager;
      Tilemap *m_tilemap;

      float m_mouseScroll = 0.0f;

      void ProcessChunksVisibility();
      void ProcessTicks();
      
      void TickHigh(float deltaTick) override;     // 20 ticks per second
      void TickMedium(float deltaTick) override;   // 1 tick per second
      void TickSlow(float deltaTick) override;     // every 30 seconds
      void TickVerySlow(float deltaTick) override; // every 2 minutes

      // UI
      bool m_debugWindow = true;
      bool m_tilesetWindow = false;

      void ShowDebugToolsMenu();

      void ShowDebugWindow();
      void ShowTilesetWindow();

    public:
      Game(Program &program);

      void Init();
      void DrawUI();
      void Draw();
      void Update(float deltaTime);
      void ProcessEvent(sf::Event &event);

      Tilemap &GetTilemap();
    };
  }
}

#endif // PX_DISASTER_GAME_HPP