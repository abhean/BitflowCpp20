#include "DrawAgent.h"
#include "DrawUtils.h"
#include "bitflow/model/Network.h"
#include "bitflow/model/Agent.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace bitflow::draw
{

namespace // unnamed namespace (internals)
{

void DrawAgentInState(sf::RenderWindow& renderWindow, model::Network const& network, model::Agent const& agent, model::Agent::Executing const& executingState)
{
  float const nodeOutlineThickness(2.0f);

  model::Node const& node = network.GetNode(executingState.NodeHandle);

  float const circleRadius(GetInfoAmountVisualRadius(agent.size));
  sf::CircleShape agentShape(circleRadius);
  agentShape.setOutlineColor(sf::Color::Red);
  agentShape.setOutlineThickness(nodeOutlineThickness);
  agentShape.setFillColor(sf::Color::Yellow);
  agentShape.setPosition(sf::Vector2f{ X(node.Position).value() - circleRadius, Y(node.Position).value() - circleRadius });
  renderWindow.draw(agentShape);
}

void DrawAgentInState(sf::RenderWindow& renderWindow, model::Network const& network, model::Agent const& agent, model::Agent::BeingTransferred const& beingTransferredState)
{
}

} // unnamed namespace (internals)

void DrawAgentVector(sf::RenderWindow& renderWindow, model::Network const& network, model::AgentVector const& agentVector)
{
  for (model::Agent const& agent : agentVector)
  {
    std::visit([&renderWindow, &network, &agent](auto&& state) { DrawAgentInState(renderWindow, network, agent, state);  }, agent.state);
  }
}

}
