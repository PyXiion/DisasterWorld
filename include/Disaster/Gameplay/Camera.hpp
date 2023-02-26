#ifndef PX_DISASTER_GAMEPLAY_CAMERA_HPP
#define PX_DISASTER_GAMEPLAY_CAMERA_HPP
#include <SFML/Graphics.hpp>
#include <Disaster/IUpdateable.hpp>

namespace px::disaster::gameplay {
  class Camera : public IUpdateable {
  private:
    sf::RenderWindow &m_window;
    sf::View m_view;
    sf::Vector2f m_position;
    float m_zoom;

  public:
    Camera(sf::RenderWindow &window);

    void Update(float deltaTime) override;

    void Move(sf::Vector2f offset);
    void Zoom(float factor);

    void SetPosition(sf::Vector2f position);
    void SetZoom(float zoom);

    sf::Vector2f GetSize() const;
    sf::Vector2f GetPosition() const;
    float GetZoom() const;
  };
}

#endif // PX_DISASTER_GAMEPLAY_CAMERA_HPP