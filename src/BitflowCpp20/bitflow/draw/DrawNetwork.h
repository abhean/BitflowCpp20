#pragma once

#include "bitflow/model/Types.h"

// Forward declarations
namespace sf
{
  class RenderWindow;
} // namespace sf

namespace bitflow::model
{
  class Network;
  class Link;
}

namespace bitflow::draw
{

model::Position GetLinkMiddlePoint(model::Network const& network, model::Link const& link);

void DrawNetwork(sf::RenderWindow& renderWindow, model::Network const& network);

}