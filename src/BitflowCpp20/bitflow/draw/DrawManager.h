#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

// Forward declarations

//
namespace bitflow::draw
{

class DrawManager
{
public:

  // Singleton
  static void Init();
  static void Done();
  static DrawManager& GetInstance();

  sf::RenderWindow& GetRenderWindow() { return m_renderWindow;  }

private:

  DrawManager();

  sf::RenderWindow m_renderWindow;

  static DrawManager* s_instance;
};

} // namespace bitflow::draw
