#include <Disaster/Program.hpp>
#include <thread>

#include <Disaster/Script/Bindings.hpp>
#include <Disaster/Utils/ResourceManager.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>
#include <easy/profiler.h>
#include <glad/glad.h>

namespace px::disaster {
  void Program::Main() {    
    m_window.Create(1280, 720, "Disaster World");
    m_window.InitGL(3, 3);
    m_window.SetFramerateLimit(60);

    // Init GLAD
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
      throw std::runtime_error("Failed to load GLAD.");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    if (!ImGui_ImplSDL2_InitForOpenGL(m_window.GetSDLHandle(), m_window.GetSDL_GLContext())) {
      throw std::runtime_error("Failed to initialise ImGui.");
    }
    if (!ImGui_ImplOpenGL3_Init("#version 330")) {
      throw std::runtime_error("Failed to initialise ImGui.");
    }

    glViewport(0, 0, m_window.GetWidth(), m_window.GetHeight());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ResourceManager::AddFolder("./data");
    ResourceManager::AddFolder("./mods");

    m_scriptEngine = new script::ScriptEngine;

    m_console = new AppConsole;

    PX_LOG("Console loaded");

    m_game = new gameplay::Game(*this);
    m_game->Init();
    
    script::ConfigureEngine(*m_scriptEngine);

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
      EASY_BLOCK("Draw UI");
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame(m_window.GetSDLHandle());
      ImGui::NewFrame();
      m_game->DrawUI();
      m_console->Draw();
      EASY_END_BLOCK;

      // Draw other
      // m_window.clear();

      EASY_BLOCK("glClear")
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      EASY_END_BLOCK;
      
      EASY_BLOCK("Draw");

      m_game->Draw();

      EASY_END_BLOCK;
      
      EASY_BLOCK("ImGui Render")
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      EASY_END_BLOCK;
      m_window.Display();
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
  script::ScriptEngine &Program::GetScriptEngine() {
    return *m_scriptEngine;
  }

  Program &Program::GetInstance() {
    static Program program;
    return program;
  }
}