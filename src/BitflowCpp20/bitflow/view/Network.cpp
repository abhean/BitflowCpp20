#include "Network.h"

namespace bitflow { namespace view {

namespace // unnamed namespace (internal)
{

sf::Vector2f PositionToVector2f(model::Position const& position)
{
  return sf::Vector2f{ X(position).value(), Y(position).value() };
}

sf::Vector2f DirectionToVector2f(model::Direction const& position)
{
  return sf::Vector2f{ X(position), Y(position) };
}

inline float GetNetworkNodeRadius(model::NetworkNode const& networkNode)
{
  return networkNode.Capacity.value();
}

void DrawLine(sf::RenderWindow& renderWindow, model::Position const& origin, model::Position const& target)
{
  sf::Vertex const vertices[] = { sf::Vertex(PositionToVector2f(origin)), sf::Vertex(PositionToVector2f(target)) };
  renderWindow.draw(vertices, 2, sf::Lines);
}

void DrawArrowEnd(sf::RenderWindow& renderWindow, model::Position const& endPosition, model::Direction const& dir, float length, float width)
{
  model::Position const arrowBase = endPosition - dir * (length * model::meter);
  model::Position const arrowSideOffset = model::Direction{ Y(dir), -X(dir) } * (width * model::meter);
  model::Position const arrowPositions[] =
  {
    endPosition,
    arrowBase + arrowSideOffset,
    arrowBase - arrowSideOffset
  };

  sf::ConvexShape arrow;
  arrow.setPointCount(3);
  for (int i = 0; i < 3; ++i)
  {
    arrow.setPoint(i, PositionToVector2f(arrowPositions[i]));
  }
  renderWindow.draw(arrow);
}

void Draw(sf::RenderWindow& renderWindow, model::NetworkNode const& networkNode)
{
  float const circleRadius(GetNetworkNodeRadius(networkNode));
  sf::CircleShape nodeShape(circleRadius);
  nodeShape.setPosition(sf::Vector2f{ X(networkNode.Position).value() - circleRadius, Y(networkNode.Position).value() - circleRadius });
  renderWindow.draw(nodeShape);
}



void Draw(sf::RenderWindow& renderWindow, model::NetworkNode const& sourceNetworkNode, model::NetworkNode const& targetNetworkNode, model::NetworkLink const& networkLink)
{
  model::Position const linkDelta  = targetNetworkNode.Position - sourceNetworkNode.Position;
  model::Length const linkLength = mag(linkDelta);
  assert(linkLength.value() > 0.0f);
  model::Direction const linkDir = linkDelta / linkLength;
  model::Position const linkOrigin = sourceNetworkNode.Position + linkDir * (GetNetworkNodeRadius(sourceNetworkNode) * model::meter);
  model::Position const linkTarget = targetNetworkNode.Position - linkDir * (GetNetworkNodeRadius(targetNetworkNode) * model::meter);

  DrawLine(renderWindow, linkOrigin, linkTarget);

  float const linkArrowLength(10.0f);
  float const linkArrowWidth(5.0f);

  DrawArrowEnd(renderWindow, linkTarget, linkDir, linkArrowLength, linkArrowWidth);
  DrawArrowEnd(renderWindow, linkOrigin, -linkDir, linkArrowLength, linkArrowWidth);
}

}// unnamed namespace (internal)


void Draw(sf::RenderWindow& renderWindow, model::Network const& network)
{
  network.ForEachNetworkNode([&renderWindow](model::NetworkNode const& networkNode) { 
    Draw(renderWindow, networkNode); 
  });

  network.ForEachNetworkLink([&renderWindow](model::NetworkNode const& sourceNetworkNode, model::NetworkNode const& targetNetworkNode, model::NetworkLink const& networkLink) { 
    Draw(renderWindow, sourceNetworkNode, targetNetworkNode, networkLink);
  });
}


} }