#pragma once

#include "bitflow/model/Agent.h"

// Forward declarations
namespace sf
{
  class RenderWindow;
} // namespace sf

namespace bitflow::model
{
  class Network;
}

namespace bitflow::draw
{

void DrawAgentVector(sf::RenderWindow& renderWindow, model::Network const& network, model::AgentVector const& agentVector);

}
