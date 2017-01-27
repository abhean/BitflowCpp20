#pragma once

#include "DebugConfig.h"
#include "bitflow/model/Types.h"
#include <SFML/Graphics/Font.hpp>
#include <spdlog/spdlog.h>
#include <memory>

namespace sf
{
  class RenderWindow;
}

namespace bitflow::debug
{

class DebugManager
{
public:

  // Singleton
  static void Init();
  static void Done();
  static DebugManager& GetInstance();

  // Log
  std::shared_ptr<spdlog::logger>& GetConsole() { return m_logger;  }
  
  // Draw
  void DrawText(model::Position const& position, std::string const& text, int const fontSize);

private:

  // Singleton
  DebugManager();

  sf::Font m_font;
  std::shared_ptr<spdlog::logger> m_logger;

  static DebugManager* s_instance;
};

} // namespace bitflow::debug