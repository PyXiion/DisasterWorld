#ifndef PX_DISASTER_SYSTEM_WINDOW_HPP
#define PX_DISASTER_SYSTEM_WINDOW_HPP
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

/// @brief 
namespace px::disaster::system {
  typedef SDL_Event Event;

  /// @brief Window
  class Window {
  public:
    /// @brief Create an uninitialised window object
    Window();

    /// Create a window with a specific width, height and title bar.
    /// @param width window width in pixels
    /// @param height window height in pixels
    /// @param title window title
    Window(int width, int height, const char *title);

    ~Window();

    /// @brief Create a window with a specific width, height and title bar.
    /// Recommended to call InitGL or InitVulkan (WIP) before calling this function
    /// @param width window width in pixels
    /// @param height window height in pixels
    /// @param title window title
    void Create(int width, int height, const char *title);

    /// @brief Closes this window and removes the OpenGL context
    void Close();

    /// @brief Initialises OpenGL. Must be called before Create
    /// @param major major OpenGL version
    /// @param minor minor OpenGL version
    void InitGL(int major, int minor);

    /// @brief Set the framerate limit. Set to 0 to remove the framerate limitation.
    /// @param fpsLimit frames per second
    void SetFramerateLimit(Uint32 fpsLimit);

    /// @brief Update a window with OpenGL rendering. This is used with double-buffered OpenGL contexts, which are the default. 
    /// There is also a delay here to adjust the frame rate.
    void Display();

    /// @brief Check that the window is open
    /// @return window openness status
    bool IsOpen() const;

    /// @brief Poll for currently pending events.
    bool PollEvent(Event &event);

    /// @brief Get the width of this window
    /// @return width of this window
    int GetWidth() const;

    /// @brief Get the height of this window
    /// @return height of this window
    int GetHeight() const;

    /// @brief Set the projection matrix. If the pointer is not set, nothing happens.
    /// @param projection projection matrix
    void SetProjection(glm::mat4 projection);

    /// @brief Set the pointer the projection matrix
    /// @param projection pointer to the projection matrix
    void SetProjectionPtr(glm::mat4 *projection);
    
    /// @brief Set the view matrix. If the pointer in not set, nothing happens.
    /// @param view view matrix
    void SetView(glm::mat4 view);

    /// @brief Set the pointer to the view matrix.
    /// @param view pointer to the view matrix
    void SetViewPtr(glm::mat4 *view);

    /// @brief Get the projection matrix. If the pointer is not set, returns zero matrix.
    /// @return projection matrix
    glm::mat4 GetProjection() const;

    /// @brief Get the pointer to the projection matrix.
    /// @return pointer to the projection matrix
    glm::mat4 *GetProjectionPtr();
    
    /// @brief Get the pointer to the projection matrix.
    /// @return pointer to the projection matrix
    const glm::mat4 *GetProjectionPtr() const;
    
    /// @brief Get the view matrix. If the pointer is not set, returns zero matrix.
    /// @return view matrix
    glm::mat4 GetView() const;

    /// @brief Get the pointer to the view matrix.
    /// @return pointer to the view matrix
    glm::mat4 *GetViewPtr();

    /// @brief Get the pointer to the view matrix.
    /// @return pointer to the view matrix
    const glm::mat4 *GetViewPtr() const;

    SDL_Window *GetSDLHandle();
    SDL_GLContext *GetSDL_GLContext();

  private:
    SDL_Window *m_window;
    SDL_GLContext m_glContext;

    bool m_running;
    Uint64 m_nextFrameTick;
    Uint32 m_framePeriod;

    int m_width;
    int m_height;

    glm::mat4 *m_projection;
    glm::mat4 *m_view;
  };
}

#endif // PX_DISASTER_SYSTEM_WINDOW_HPP