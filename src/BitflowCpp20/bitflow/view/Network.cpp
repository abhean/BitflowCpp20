#include "Network.h"

namespace bitflow { namespace view {

namespace // unnamed namespace (internal)
{

inline float GetNetworkNodeRadius(model::NetworkNode const& networkNode)
{
  return networkNode.Capacity.value();
}

}// unnamed namespace (internal)


void Draw(sf::RenderWindow& renderWindow, model::Network const& network)
{
  // Draw network nodes
  network.ForEachNetworkNode( [&renderWindow] (model::NetworkNode const& networkNode) {
    float const circleRadius(GetNetworkNodeRadius(networkNode));
    sf::CircleShape nodeShape(circleRadius);
    nodeShape.setPosition(sf::Vector2f{ X(networkNode.Position).value() - circleRadius, Y(networkNode.Position).value() - circleRadius });
    renderWindow.draw(nodeShape);
  });

  // Draw network links
  network.ForEachNetworkLink( [&renderWindow] (model::NetworkNode const& sourceNetworkNode, model::NetworkNode const& targetNetworkNode, model::NetworkLink const& networkLink) {
    model::Position  const linkDelta  = targetNetworkNode.Position - sourceNetworkNode.Position;
    model::Length    const linkLength = mag(linkDelta);
    assert(linkLength.value() > 0.0f);
    model::Direction const linkDir    = linkDelta / linkLength;
    model::Position  const linkOrigin = sourceNetworkNode.Position + linkDir * (GetNetworkNodeRadius(sourceNetworkNode) * model::meter);
    model::Position  const linkTarget = targetNetworkNode.Position - linkDir * (GetNetworkNodeRadius(targetNetworkNode) * model::meter);

    sf::Vertex const lineVertices[] = { sf::Vertex(sf::Vector2f(X(linkOrigin).value(), Y(linkOrigin).value())), sf::Vertex(sf::Vector2f(X(linkTarget).value(), Y(linkTarget).value())) };
    renderWindow.draw(lineVertices, 2, sf::Lines);
  });
}


} }