#pragma once

#include "bitflow/model/Types.h"
#include <SFML/Graphics.hpp>

namespace bitflow::view 
{

sf::Vector2f PositionToVector2f(model::Position const& position)
{
  return sf::Vector2f{ X(position).value(), Y(position).value() };
}

sf::Vector2f DirectionToVector2f(model::Direction const& position)
{
  return sf::Vector2f{ X(position), Y(position) };
}

}
