#include "DrawWorld.h"
#include "DrawNetwork.h"
#include "bitflow/model/Agent.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace bitflow::draw
{


namespace // unnamed namespace (internals)
{
float GetAgentVisualSize(model::Agent const& agent)
{
  return sqrtf(agent.size.value());
}

void DrawAgentInLocation(sf::RenderWindow& renderWindow, model::Network const& network, model::Agent const& agent, model::Agent::InNode const& inNode)
{
  float const nodeOutlineThickness(2.0f);

  float const agentVisualSize(GetAgentVisualSize(agent));
  float const agentHalfVisualSize(agentVisualSize * 0.5f);
  sf::RectangleShape agentShape(sf::Vector2f{ agentVisualSize, agentVisualSize });
  agentShape.setOutlineColor(sf::Color::White);
  agentShape.setOutlineThickness(nodeOutlineThickness);
  agentShape.setFillColor(sf::Color::White);
  model::Node const& node = network.GetNode(inNode.NodeHandle);
  agentShape.setPosition(sf::Vector2f{ X(node.Position).value() - agentHalfVisualSize, Y(node.Position).value() - agentHalfVisualSize });
  renderWindow.draw(agentShape);
}

void DrawAgentInLocation(sf::RenderWindow& renderWindow, model::Network const& network, model::Agent const& agent, model::Agent::InLink const& inLink)
{
}

void DrawAgentVector(sf::RenderWindow& renderWindow, model::Network const& network, model::AgentVector const& agentVector)
{
  for (model::Agent const& agent : agentVector)
  {
    std::visit([&renderWindow, &network, &agent](auto&& location) { DrawAgentInLocation(renderWindow, network, agent, location);  }, agent.location);
  }
}

} // unnamed namespace (internals)

void Draw(sf::RenderWindow& renderWindow, model::World const & world)
{
  Draw(renderWindow, world.Network);
  DrawAgentVector(renderWindow, world.Network, world.Agents);
}

#if DEBUG_DRAW
void DrawDebug(sf::RenderWindow& renderWindow, model::World const & world)
{
  DrawDebug(renderWindow, world.Network);
  DrawDebugAgentVector(renderWindow, world.Network, world.Agents);
}
#endif // DEBUG_DRAW

}
