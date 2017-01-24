#pragma once

// Forward declarations
namespace sf
{
  class RenderWindow;
} // namespace sf

namespace bitflow::model
{
  class Network;
}

namespace bitflow::view
{

void Draw(sf::RenderWindow& renderWindow, model::Network const& network);

}