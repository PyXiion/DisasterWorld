#ifndef PX_DISASTER_GAME_HPP
#define PX_DISASTER_GAME_HPP
#include <Disaster/ITickable.hpp>
#include <Disaster/Gameplay/World.hpp>
#include <Disaster/Gameplay/Camera.hpp>
#include <Disaster/Gameplay/GameSettings.hpp>
#include <Disaster/Gameplay/Tilemap.hpp>
#include <Disaster/System/Window.hpp>
#include <Disaster/Utils/TextureManager.hpp>

namespace px::disaster {
  class Program;

  namespace gameplay {
    class Game : private ITickable {
    private:
      Program &m_program;
      system::Window &m_window;
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

      /// @brief Sends a request to load/generate chunks to m_world, which go into the camera view. Deletes chunks out of view.
      void ProcessChunksVisibility();

      /// @brief Calls TickHight, TickMedium, TickSlow, TickVerySlow periodically
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

      /// @brief Loading textures, initializing the world and tileset.
      void Init();

      /// @brief Method where ImGui functions should be called
      void DrawUI();

      /// @brief Method where OpenGL draw calls should be
      void Draw();

      /// @brief This method is called every frame
      /// @param deltaTime time between frames
      void Update(float deltaTime);
      void ProcessEvent(system::Event &event);

      Tilemap &GetTilemap();
    };
  }
}

#endif // PX_DISASTER_GAME_HPP