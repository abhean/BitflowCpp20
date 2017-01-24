#include "NetworkView.h"
#include "ViewUtils.h"
#include "bitflow/model/Network.h"
#include "bitflow/model/Frame.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace bitflow { namespace view {

namespace // unnamed namespace (internal)
{

inline float GetNetworkNodeRadius(model::Node const& networkNode)
{
  return networkNode.Capacity.value();
}

void DrawLine(sf::RenderWindow& renderWindow, sf::Color const& color, model::Position const& origin, model::Position const& target)
{
  sf::Vertex const vertices[] = { sf::Vertex(PositionToVector2f(origin), color), sf::Vertex(PositionToVector2f(target), color) };
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

void DrawNode(sf::RenderWindow& renderWindow, model::Node const& networkNode)
{
  float const nodeOutlineThickness(2.0f);

  float const circleRadius(GetNetworkNodeRadius(networkNode));
  sf::CircleShape nodeShape(circleRadius);
  nodeShape.setOutlineColor(sf::Color::Blue);
  nodeShape.setOutlineThickness(nodeOutlineThickness);
  nodeShape.setFillColor(sf::Color::Transparent);
  nodeShape.setPosition(sf::Vector2f{ X(networkNode.Position).value() - circleRadius, Y(networkNode.Position).value() - circleRadius });
  renderWindow.draw(nodeShape);
}

void DrawLink(sf::RenderWindow& renderWindow, model::Node const& sourceNetworkNode, model::Node const& targetNetworkNode, model::Link const& networkLink)
{
  model::Position const linkDelta  = targetNetworkNode.Position - sourceNetworkNode.Position;
  model::Length const linkLength = mag(linkDelta);
  assert(linkLength.value() > 0.0f);
  model::Direction const linkDir = linkDelta / linkLength;
  model::Length const sourceNetworkNodeVisualRadius = GetNetworkNodeRadius(sourceNetworkNode) * model::meter;
  model::Length const targetNetworkNodeVisualRadius = GetNetworkNodeRadius(targetNetworkNode) * model::meter;
  model::Position const linkOrigin = sourceNetworkNode.Position + linkDir * sourceNetworkNodeVisualRadius;
  model::Position const linkTarget = targetNetworkNode.Position - linkDir * targetNetworkNodeVisualRadius;

  DrawLine(renderWindow, sf::Color::White, linkOrigin, linkTarget);

  float const linkArrowLength(10.0f);
  float const linkArrowWidth(5.0f);

  DrawArrowEnd(renderWindow, linkTarget, linkDir, linkArrowLength, linkArrowWidth);
  DrawArrowEnd(renderWindow, linkOrigin, -linkDir, linkArrowLength, linkArrowWidth);

  // Draw frames
  float const logicLinkToVisualLinkLength = (linkLength - sourceNetworkNodeVisualRadius - targetNetworkNodeVisualRadius) / linkLength;

  for (model::Frame const& frame : networkLink.FrameQueue)
  {
    model::Position const frameOrigin = linkOrigin + linkDir * (fmax(0.0f, boost::units::quantity_cast<float>(frame.LinkPos)) * logicLinkToVisualLinkLength * model::meter);
    model::Position const frameEnd    = linkOrigin + linkDir * (fmin(boost::units::quantity_cast<float>(networkLink.GetLength()), boost::units::quantity_cast<float>(frame.LinkPos + frame.Length)) * logicLinkToVisualLinkLength * model::meter);

    // Normalize position 
    DrawLine(renderWindow, sf::Color::Red, frameOrigin, frameEnd);
  }
}

}// unnamed namespace (internal)


void Draw(sf::RenderWindow& renderWindow, model::Network const& network)
{
  network.ForEachNetworkNode([&renderWindow](model::Node const& networkNode) { 
    DrawNode(renderWindow, networkNode); 
  });

  network.ForEachNetworkLink([&renderWindow](model::Node const& sourceNetworkNode, model::Node const& targetNetworkNode, model::Link const& networkLink) { 
    DrawLink(renderWindow, sourceNetworkNode, targetNetworkNode, networkLink);
  });
}


} }