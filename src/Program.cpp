#include <Disaster/Program.hpp>
#include <thread>

#include <imgui-sfml/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <easy/profiler.h>

namespace px::disaster {
  void Program::Main() {    
    m_window.create(sf::VideoMode(1280, 720), "Disaster World");
    m_window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(m_window)) {
      throw std::runtime_error("Failed to initialise ImGui.");
    }

    m_scriptEngine = new script::ScriptEngine;
    m_console = new AppConsole;

    PX_LOG("Console loaded");

    m_textureManager = new utils::TextureManager;

    m_game = new gameplay::Game(*this);
    m_game->Init();

    // Some debug data
    PX_LOG("Max real thread count: %d", std::thread::hardware_concurrency());


    sf::Clock clock;
    while (m_window.isOpen()) {
      EASY_BLOCK("Frame");
      sf::Time _deltaTime = clock.restart();
      float deltaTime = _deltaTime.asSeconds();

      EASY_BLOCK("Catch events");
      sf::Event event;
      while (m_window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(m_window, event);
        m_game->ProcessEvent(event);
      }
      EASY_END_BLOCK;

      // Update
      EASY_BLOCK("Update");
      m_game->Update(deltaTime);
      ImGui::SFML::Update(m_window, _deltaTime);
      EASY_END_BLOCK;

      // Draw UI
      EASY_BLOCK("Draw UI");
      m_game->DrawUI();
      m_console->Draw();
      EASY_END_BLOCK;

      // Draw other
      m_window.clear();
      EASY_BLOCK("Draw");

      m_game->Draw();
      ImGui::SFML::Render(m_window);

      EASY_END_BLOCK;
      m_window.display();
      
      EASY_END_BLOCK;
    }
  }

  sf::RenderWindow &Program::GetWindow() {
    return m_window;
  }
  gameplay::Game &Program::GetGame() {
    return *m_game;
  }
  utils::TextureManager &Program::GetTextureManager() {
    return *m_textureManager;
  }
  script::ScriptEngine &Program::GetScriptEngine() {
    return *m_scriptEngine;
  }

  Program &Program::GetInstance() {
    static Program program;
    return program;
  }
}