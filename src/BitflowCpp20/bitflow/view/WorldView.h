#pragma once

#include "bitflow\model\World.h"

// Forward declarations
namespace sf
{
  class RenderWindow;
} // namespace sf

namespace bitflow::view
{

void Draw(sf::RenderWindow& renderWindow, model::World const& world);

} // namespace bitflow::view