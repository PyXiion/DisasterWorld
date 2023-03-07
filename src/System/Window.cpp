#include <Disaster/System/Window.hpp>
#include <thread>

namespace px::disaster::system {
  Window::Window() : m_window(0), m_glContext(0), m_running(0), m_nextFrameTick(0), m_framePeriod(0) {

  }
  Window::Window(int width, int height, const char *title) {
    Create(width, height, title);
  }
  Window::~Window() {
    Close();
  }

  void Window::Create(int width, int height, const char *title) {
    static char _once = [] {
      SDL_Init(SDL_INIT_VIDEO);
      return 0;
    }();
    m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    m_glContext = SDL_GL_CreateContext(m_window);

    m_running = true;

    m_width = width;
    m_height = height;
  }
  void Window::Close() {
    m_running = false;

    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
  }

  void Window::InitGL(int major, int minor) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  }

  void Window::SetFramerateLimit(Uint32 fpsLimit) {
    m_framePeriod = 1000 / fpsLimit;
  }

  void Window::Display() {
    // FPS limit
    if (m_framePeriod) {
      Uint64 currentTick = SDL_GetTicks64();
      if (currentTick < m_nextFrameTick) {
        // Wait next frame
        std::this_thread::sleep_for(std::chrono::milliseconds(m_nextFrameTick - currentTick));
        m_nextFrameTick = currentTick + m_framePeriod;
      }
    }

    SDL_GL_SwapWindow(m_window);
  }

  bool Window::IsOpen() const {
    return m_running;
  }
  bool Window::PollEvent(SDL_Event &event) {
    return SDL_PollEvent(&event);
  }

  int Window::GetWidth() const {
    return m_width;
  }
  int Window::GetHeight() const {
    return m_height;
  }
  Vector2i Window::GetSize() const {
    return Vector2i(m_width, m_height);
  }

  void Window::SetProjection(glm::mat4 projection) {
    if (m_projection) *m_projection = projection;
  }
  void Window::SetProjectionPtr(glm::mat4 *projection) {
    m_projection = projection;
  }
  
  void Window::SetView(glm::mat4 view) {
    if (m_view) *m_view = view;
  }
  void Window::SetViewPtr(glm::mat4 *view) {
    m_view = view;
  }
  
  glm::mat4 Window::GetProjection() const {
    return (m_projection) ? *m_projection : glm::mat4(0.0f);
  }
  glm::mat4 *Window::GetProjectionPtr() {
    return m_projection;
  }
  const glm::mat4 *Window::GetProjectionPtr() const {
    return m_projection;
  }
  
  glm::mat4 Window::GetView() const {
    return (m_view) ? *m_view : glm::mat4(0.0f);
  }
  glm::mat4 *Window::GetViewPtr() {
    return m_view;
  }
  const glm::mat4 *Window::GetViewPtr() const {
    return m_view;
  }

  SDL_Window *Window::GetSDLHandle() {
    return m_window;
  }
  SDL_GLContext *Window::GetSDL_GLContext() {
    return &m_glContext;
  }
}