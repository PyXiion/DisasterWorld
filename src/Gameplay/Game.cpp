#include <Disaster/Gameplay/Game.hpp>

#include <filesystem>

#include <Disaster/Program.hpp>
#include <Disaster/Graphics/Sprite.hpp>
#include <Disaster/Graphics/VertexArray.hpp>
#include <Disaster/Utils/RamUsage.hpp>
#include <Disaster/Utils/ResourceManager.hpp>
#include <imgui/imgui.h>
#include <easy/profiler.h>
#include <easy/arbitrary_value.h>

namespace fs = std::filesystem;

namespace px::disaster::gameplay {
  Game::Game(Program &program) 
    : m_program(program), 
      m_window(program.GetWindow()), 
      m_scriptEngine(program.GetScriptEngine()),
      m_tickSpeed(1.0f), 
      m_tickMask(0), 
      m_mouseScroll(0.0f) {
    m_tickRates = {1.0f/20.0f, 1.0f, 30.0f, 120.0f};
    m_tickFunctions = {&Game::TickHigh, &Game::TickMedium, &Game::TickSlow, &Game::TickVerySlow};
  }

  void Game::Init() {
    EASY_BLOCK("Game::Init", profiler::colors::Blue700);
    // Load all core textures
    PX_LOG("Loading Core textures");

    m_tilemap = new Tilemap();
    m_tilemap->LoadTilemap("./mods/core/tilemaps/general.yaml");

    m_camera = new Camera();
    // m_camera->SetZoom(5.0f);

    PX_LOG("World is initialising");
    m_world = new World();
    PX_LOG("World initialised");
  }

  void Game::DrawUI() {
    EASY_BLOCK("Game::DrawUI", profiler::colors::Yellow700);
    if (m_debugWindow) ShowDebugWindow();
    if (m_tilesetWindow) ShowTilesetWindow();
  }
  void Game::ShowDebugWindow() {
    EASY_BLOCK("Game::ShowDebugWindow");
    ImGui::Begin("Debug", &m_debugWindow, ImGuiWindowFlags_MenuBar);

    // Menu bar
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("Tools")) {
        ShowDebugToolsMenu();
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    // Debug information
    if (ImGui::CollapsingHeader("Debug information", ImGuiTreeNodeFlags_DefaultOpen)) {
      
      ImGui::Text("FPS: %.2f", 1.0f / m_deltaTime);
      
      size_t ram = utils::GetUsedMemory();
      ImGui::Text("RAM: %luM", ram / 1024lu / 1024lu);

      if (ImGui::TreeNode("Camera")) {
        Vector2f camPos = m_camera->GetPosition();
        Vector2f camSize = m_camera->GetSize();
        // ImGui::Text("Position: %.1f %.1f", camPos.x, camPos.y);
        if (ImGui::InputFloat2("Position", &camPos.x, "%.0f", ImGuiInputTextFlags_EnterReturnsTrue)) {
          m_camera->SetPosition(camPos);
        }
        ImGui::Text("Size: %.1f %.1f", camSize.x, camSize.y);
        ImGui::Text("Zoom: %.2f", m_camera->GetZoom());

        ImGui::TreePop();
      }
      if (ImGui::TreeNode("Chunks")) {
        ImGui::Text("Chunks queue size = %lu", m_world->m_chunkRequests.Size());
        int i = 0;
        m_world->IterateChunks([&] (ChunkPtr &elem) {
          Vector2i pos = elem->GetPosition();
          ImGui::Text("#%d at %d %d, loaded=%d, visisble=%d, ref count=%lu", 
              i++, pos.x, pos.y, 
              elem->IsLoaded(), 
              m_camera->IsInView(elem->GetBounds().Convert<float>()),
              elem.use_count());
        });
        ImGui::TreePop();
      }
    }
    ImGui::End();
  }
  void Game::ShowDebugToolsMenu() {
    EASY_FUNCTION();
    // World tools
    ImGui::MenuItem("World", nullptr, false, false);
    // Tile editor
    ImGui::MenuItem("Edit tiles (in progress)");


    // Other tools
    ImGui::MenuItem("Other", nullptr, false, false);
    // Tileset
    ImGui::MenuItem("Tileset", nullptr, &m_tilesetWindow);
    // Console
    ImGui::MenuItem("Console", nullptr, &AppConsole::GetInstance()->m_open);
  }
  void Game::ShowTilesetWindow() {
    EASY_FUNCTION();
    ImGui::Begin("Tileset", &m_tilesetWindow);
    if (ImGui::CollapsingHeader("Tileset texture"))
      // ImGui::Image(m_tilemap->GetTilemapTexture());
    if (ImGui::CollapsingHeader("Tiles")) {
      for (const auto &tile : m_tilemap->GetTiles()) {
        if (ImGui::TreeNode(tile.id.c_str())) {
          ImGui::Text("Texture ID: %s", tile.textureId.c_str());
          ImGui::Text("Tile rect: %i %i %i %i", tile.tilesetRect.left, tile.tilesetRect.top, 
              tile.tilesetRect.width, tile.tilesetRect.height);
          // ImGui::Image(*tile.texture);
          ImGui::TreePop();
        }
      }
    }
    ImGui::End();
  }

  void Game::Draw() {
    EASY_BLOCK("Game::Draw", profiler::colors::Green700);

    m_world->Draw();
  }

  void Game::Update(float deltaTime) {
    EASY_BLOCK("Game::Update", profiler::colors::Red700);
    m_deltaTime = deltaTime;

    ProcessTicks();
    ProcessChunksVisibility();

    ImGuiIO &imguiIO = ImGui::GetIO();

    // Move camera
    if (!imguiIO.WantCaptureMouse && !imguiIO.WantCaptureKeyboard) {
      float camZoom = m_camera->GetZoom();
      Vector2f offset;
      if (m_keyboard[SDLK_w])
        offset.y += m_settings.cameraSpeed;
      if (m_keyboard[SDLK_s])
        offset.y -= m_settings.cameraSpeed;
      if (m_keyboard[SDLK_d])
        offset.x += m_settings.cameraSpeed;
      if (m_keyboard[SDLK_a])
        offset.x -= m_settings.cameraSpeed;

      if (m_keyboard[SDLK_e])
        m_camera->Zoom(1.0f + (0.1f * deltaTime));
      if (m_keyboard[SDLK_q])
        m_camera->Zoom(1.0f - (0.1f * deltaTime));

      if (m_mouseScroll > 0)
        m_camera->SetZoom(camZoom + deltaTime);
      else if (m_mouseScroll < 0)
        m_camera->SetZoom(camZoom - deltaTime);
      
      offset = offset / camZoom * deltaTime;
      m_camera->Move(offset);
      m_mouseScroll = 0.0f;
    }

    m_camera->Update();
  }

  void Game::ProcessEvent(system::Event &event) {
    EASY_BLOCK("Game::ProcessEvent");
    switch (event.type) {
      case SDL_MOUSEWHEEL: {
        m_mouseScroll = event.wheel.y;
        break;
      }
      case SDL_KEYDOWN: {
        m_keyboard[event.key.keysym.sym] = true;
        break;
      }
      case SDL_KEYUP: {
        m_keyboard[event.key.keysym.sym] = false;
        break;
      }
      default:
        break;
    }
  }
  void Game::ProcessChunksVisibility() {
    EASY_BLOCK("Game::ProcessChunksVisibility");

    DeleteChunksOutOfSight();
    LoadChunksInSight();
  }
  void Game::LoadChunksInSight() {
    EASY_BLOCK("Game::LoadChunksInSight");
    FloatRect cameraViewBounds = m_camera->GetViewBounds();
    Vector2f leftTop(cameraViewBounds.left, cameraViewBounds.top);
    Vector2f rightBottom = leftTop + Vector2f(cameraViewBounds.width, cameraViewBounds.height);

    leftTop /= static_cast<float>(kChunkSize);
    rightBottom /= static_cast<float>(kChunkSize);

    for (int x = std::floor(leftTop.x); x <= std::floor(rightBottom.x); x++) {
      for (int y = std::floor(leftTop.y); y <= std::floor(rightBottom.y); y++) {
        if (m_world->RequestChunk({x, y})) {
          return;
        }
      }
    }
  }
  void Game::DeleteChunksOutOfSight() {
    EASY_BLOCK("Game::DeleteChunksOutOfSight");
    m_world->DeleteChunksIf([this](ChunkPtr &chunk) { return !m_camera->IsInView(chunk->GetBounds().Convert<float>()); });
  }

  void Game::ProcessTicks() {
    EASY_BLOCK("Game::ProcessTicks");
    // Update elapsed times
    for (int i = 0; i < 4; i++) {
      m_elapsedTimes[i] += m_deltaTime;
      if (m_elapsedTimes[i] >= m_tickRates[i]) {
        m_tickMask |= (1 << i); // set bit for tick function i
        m_elapsedTimes[i] -= m_tickRates[i];
      }
    }

    // Call tick functions based on tick mask
    for (int i = 0; i < 4; i++) {
      if (m_tickMask & (1 << i)) {
        (this->*m_tickFunctions[i])(m_tickRates[i] * m_tickSpeed);
        m_tickMask &= ~(1 << i); // clear bit for tick function i
      }
    }
  }
  void Game::TickHigh(float deltaTick) {
    EASY_FUNCTION();
  }
  void Game::TickMedium(float deltaTick) {
    EASY_FUNCTION();
  }
  void Game::TickSlow(float deltaTick) {
    EASY_FUNCTION();
  }
  void Game::TickVerySlow(float deltaTick) {
    EASY_FUNCTION();
  }

  Tilemap &Game::GetTilemap() {
    return *m_tilemap;
  }
  World &Game::GetWorld() {
    return *m_world;
  }
}