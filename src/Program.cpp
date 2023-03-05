#include <Disaster/Program.hpp>
#include <thread>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>
#include <easy/profiler.h>
#include <glad/glad.h>

namespace px::disaster {
  void Program::Main() {    
    m_window.InitGL(4, 5);
    m_window.Create(1280, 720, "Disaster World");
    m_window.SetFramerateLimit(60);

    // Init GLAD
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
      throw std::runtime_error("Failed to load GLAD.");
    }
    glViewport(0, 0, m_window.GetWidth(), m_window.GetHeight());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    if (!ImGui_ImplSDL2_InitForOpenGL(m_window.GetSDLHandle(), m_window.GetSDL_GLContext())) {
      throw std::runtime_error("Failed to initialise ImGui.");
    }
    if (!ImGui_ImplOpenGL3_Init("#version 450")) {
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

    Uint64 lastTick = 0;
    while (m_window.IsOpen()) {
      EASY_BLOCK("Frame");
      Uint64 currentTick = SDL_GetTicks64();
      float deltaTime = (currentTick - lastTick) / 1000.0f;
      lastTick = currentTick;

      EASY_BLOCK("Catch events");
      system::Event event;
      while (m_window.PollEvent(event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
          m_window.Close();
        m_game->ProcessEvent(event);
      }
      EASY_END_BLOCK;

      // Update
      EASY_BLOCK("Update");
      m_game->Update(deltaTime);
      EASY_END_BLOCK;

      // Draw UI
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame(m_window.GetSDLHandle());
      ImGui::NewFrame();
      EASY_BLOCK("Draw UI");
      m_game->DrawUI();
      m_console->Draw();
      EASY_END_BLOCK;

      // Draw other
      // m_window.clear();
      EASY_BLOCK("Draw");
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      // m_game->Draw();
      
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      EASY_END_BLOCK;
      m_window.Display();

      EASY_END_BLOCK;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
  }

  system::Window &Program::GetWindow() {
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