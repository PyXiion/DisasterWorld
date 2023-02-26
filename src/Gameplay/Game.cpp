#include <Disaster/Gameplay/Game.hpp>

#include <filesystem>

#include <Disaster/Program.hpp>
#include <Disaster/Utils/RamUsage.hpp>
#include <imgui-sfml/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <easy/profiler.h>
#include <easy/arbitrary_value.h>

namespace fs = std::filesystem;

namespace px::disaster::gameplay {
  Game::Game(Program &program) : m_program(program), m_window(program.GetWindow()), m_textureManager(program.GetTextureManager()) {
    m_tickRates = {1.0f/20.0f, 1.0f, 30.0f, 120.0f};
    m_tickFunctions = {&Game::TickHigh, &Game::TickMedium, &Game::TickSlow, &Game::TickVerySlow};
  }

  void Game::Init() {
    EASY_BLOCK("Game::Init", profiler::colors::Blue700);
    // Load all core textures
    PX_LOG("Loading Core textures");
    for (auto const &entry : fs::recursive_directory_iterator("./mods/core/textures")) {
      if (!fs::is_regular_file(entry.path()))
        continue;
      m_textureManager.LoadTexture(entry.path());
    }

    m_tilemap = new Tilemap(m_textureManager);
    m_tilemap->LoadTilemap("./mods/core/tilemaps/general.yaml");

    EASY_EVENT("Resources loaded", profiler::colors::Magenta);

    m_camera = new Camera(m_window);
    m_camera->SetZoom(5.0f);

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
    EASY_FUNCTION();
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

      // Camera
      if (ImGui::TreeNode("Camera")) {
        sf::Vector2f camPos = m_camera->GetPosition();
        sf::Vector2f camSize = m_camera->GetSize();
        ImGui::Text("Position: %f %f", camPos.x, camPos.y);
        ImGui::Text("Size: %f %f", camSize.x, camSize.y);
        ImGui::Text("Zoom: %f", m_camera->GetZoom());

        ImGui::TreePop();
      }
      // Chunks queue
      if (ImGui::TreeNode("Chunks queue")) {
        std::lock_guard<std::mutex> lk(m_world->m_chunksQueueMutex);
        std::queue tmp_q = m_world->m_chunkQueue; //copy the original queue to the temporary queue

        while (!tmp_q.empty())
        {
          auto elem = tmp_q.front();
          ImGui::Text("Chunk %p", elem);
          tmp_q.pop();
        } 
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
      ImGui::Image(m_tilemap->GetTilemapTexture());
    if (ImGui::CollapsingHeader("Tiles")) {
      for (const auto &tile : m_tilemap->GetTiles()) {
        if (ImGui::TreeNode(tile.id.c_str())) {
          ImGui::Text("Texture ID: %s", tile.textureId.c_str());
          ImGui::Text("Tile rect: %i %i %i %i", tile.tilemapRect.left, tile.tilemapRect.top, 
              tile.tilemapRect.width, tile.tilemapRect.height);
          ImGui::Image(*tile.texture);
          ImGui::TreePop();
        }
      }
    }
    ImGui::End();
  }

  void Game::Draw() {
    EASY_BLOCK("Game::Draw", profiler::colors::Green700);
    m_window.draw(*m_world);

    // sf::Sprite tilemap(m_tilemap->GetTilemapTexture());
    // m_window.draw(tilemap);
  }
  void Game::Update(float deltaTime) {
    EASY_FUNCTION(profiler::colors::Red700);
    m_deltaTime = deltaTime;

    ProcessTicks();
    ProcessChunksVisibility();

    ImGuiIO &imguiIO = ImGui::GetIO();

    // Move camera
    if (!imguiIO.WantCaptureMouse && !imguiIO.WantCaptureKeyboard) {
      float camZoom = m_camera->GetZoom();
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        m_camera->Move(sf::Vector2f(0, -m_settings.cameraSpeed / camZoom * deltaTime));
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        m_camera->Move(sf::Vector2f(0, m_settings.cameraSpeed / camZoom * deltaTime));
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        m_camera->Move(sf::Vector2f(-m_settings.cameraSpeed / camZoom * deltaTime, 0));
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        m_camera->Move(sf::Vector2f(m_settings.cameraSpeed / camZoom * deltaTime, 0));
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        m_camera->Zoom(1.0f + (0.1f * deltaTime));
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        m_camera->Zoom(1.0f - (0.1f * deltaTime));

      if (m_mouseScroll > 0)
        m_camera->Zoom(1.1f);
      else if (m_mouseScroll < 0)
        m_camera->Zoom(0.9f);
      m_mouseScroll = 0.0f;
    }

    m_camera->Update(deltaTime);

    EASY_VALUE("CameraZoom", m_camera->GetZoom(), EASY_VIN("CameraZoom"));
  }

  void Game::ProcessEvent(sf::Event &event) {
    EASY_FUNCTION();
    switch (event.type) {
      case sf::Event::Closed: {
        m_window.close();
        break;
      }
      case sf::Event::MouseWheelScrolled: {
        m_mouseScroll = event.mouseWheelScroll.delta;
      }
      default:
        break;
    }
  }
  void Game::ProcessChunksVisibility() {
    EASY_FUNCTION();
    std::unique_lock<std::mutex> lk(m_world->GetChunksMutex());
    auto &chunks = m_world->GetChunks();
    sf::Vector2f camPos = m_camera->GetPosition() / static_cast<float>(kChunkSize);
    sf::Vector2f camSize = m_camera->GetSize() / 2.0f / static_cast<float>(kChunkSize);
    camPos.x = std::floor(camPos.x);
    camPos.y = std::floor(camPos.y);

    camSize.x = std::ceil(camSize.x);
    camSize.y = std::ceil(camSize.y);

    // Unload chunks that are too far from the camera position
    EASY_BLOCK("Unload chunks");
    for (auto it = chunks.begin(); it != chunks.end(); ) {
      int chunkX = (*it)->GetX();
      int chunkY = (*it)->GetY();
      if (!(*it)->IsInQueue() && 
       (static_cast<int>(std::floor(std::abs(camPos.x - chunkX))) > camSize.x ||
        static_cast<int>(std::floor(std::abs(camPos.y - chunkY))) > camSize.y)) {
        // it->Unload();
        it = chunks.erase(it);
        PX_LOG("Chunk %d %d deleted", chunkX, chunkY);
      } 
      else {
        ++it;
      }
    }
    EASY_END_BLOCK;
    lk.unlock();

    // Load chunks that are close to the camera position
    EASY_BLOCK("Load chunks");
    for (int x = std::floor(camPos.x - camSize.x); x <= std::ceil(camPos.x + camSize.x); x++) {
      std::lock(m_world->m_chunksQueueMutex, m_world->m_chunksMutex);
      std::lock_guard<std::mutex> lk_a(m_world->m_chunksQueueMutex, std::adopt_lock);
      std::lock_guard<std::mutex> lk_b(m_world->m_chunksMutex, std::adopt_lock);
      for (int y = std::floor(camPos.y - camSize.y); y <= std::ceil(camPos.y + camSize.y); y++) {
        m_world->RequestChunkUnsafe(x, y);
      }
    }
    EASY_END_BLOCK;
  }

  void Game::ProcessTicks() {
    EASY_FUNCTION();
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
}