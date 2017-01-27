#pragma once

// Forward declarations
namespace sf
{
  class RenderWindow;
} // namespace sf

namespace bitflow::model
{
  struct Agent;
  class Network;
}

namespace bitflow::draw
{

void Draw(sf::RenderWindow& renderWindow, model::Network const& network, model::Agent const& agent);

}
