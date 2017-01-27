#include "DebugManager.h"
#include "bitflow/draw/DrawUtils.h"
#include "bitflow/draw/DrawManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

namespace bitflow::debug
{

namespace // unnamed namespace (internals)
{

} // unnamed namespace (internals)

DebugManager* DebugManager::s_instance = nullptr;

DebugManager::DebugManager()
{
  m_logger = spdlog::stdout_color_mt("console");

  m_font.loadFromFile("assets/WHITRABT.TTF");
}

void DebugManager::Init()
{
  assert(!s_instance);
  s_instance = new DebugManager();
}

void DebugManager::Done()
{
  assert(s_instance);
  delete s_instance;
  s_instance = nullptr;
}

DebugManager& DebugManager::GetInstance()
{
  assert(s_instance);
  return *s_instance;
}

void DebugManager::DrawText(model::Position const& position, std::string const& text, int const fontSize)
{
  sf::Text textShape(text, m_font);
  textShape.setPosition(draw::PositionToVector2f(position));
  textShape.setCharacterSize(fontSize);

  draw::DrawManager::GetInstance().GetRenderWindow().draw(textShape);
}

} // namespace bitflow::debug