#pragma once

#include "bitflow/model/Types.h"
#include <SFML/Graphics.hpp>

namespace bitflow::draw 
{

inline sf::Vector2f PositionToVector2f(model::Position const& position)
{
  return sf::Vector2f{ X(position).value(), Y(position).value() };
}

inline sf::Vector2f DirectionToVector2f(model::Direction const& position)
{
  return sf::Vector2f{ X(position), Y(position) };
}

// Auxiliary functions
float GetInfoAmountVisualRadius(model::Info const& info);

}
