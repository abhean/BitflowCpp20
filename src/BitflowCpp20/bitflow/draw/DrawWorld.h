#pragma once

#include "bitflow\model\World.h"

// Forward declarations
namespace sf
{
  class RenderWindow;
} // namespace sf

namespace bitflow::draw
{

void DrawWorld(sf::RenderWindow& renderWindow, model::World const& world);

} // namespace bitflow::draw