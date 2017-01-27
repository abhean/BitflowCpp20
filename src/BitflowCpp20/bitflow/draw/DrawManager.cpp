#include "DrawManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cassert>

namespace bitflow::draw
{

DrawManager* DrawManager::s_instance = nullptr;

void DrawManager::Init()
{
  assert(!s_instance);
  s_instance = new DrawManager();
}

void DrawManager::Done()
{
  assert(s_instance);
  delete s_instance;
  s_instance = nullptr;
}

DrawManager& DrawManager::GetInstance()
{
  assert(s_instance);
  return *s_instance;
}

DrawManager::DrawManager()
  : m_renderWindow(sf::VideoMode(1000, 1000), "my window")
{
  m_renderWindow .setFramerateLimit(60u);
  //m_renderWindow.setVerticalSyncEnabled(true);
}

} // namespace bitflow::draw