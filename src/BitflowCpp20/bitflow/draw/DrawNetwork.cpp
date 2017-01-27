#include "DrawNetwork.h"
#include "DrawUtils.h"
#include "bitflow/model/Network.h"
#include "bitflow/model/Frame.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <boost/math/constants/constants.hpp>

namespace bitflow::draw {

namespace // unnamed namespace (internal)
{

// Constants
float const kLinkArrowLength(10.0f);
float const kLinkArrowWidth(5.0f);

// Auxiliary functions
inline float GetNetworkNodeRadius(model::Node const& networkNode)
{
  float const nodeSurfaceToVisualRadius = 10.0f;
  return nodeSurfaceToVisualRadius * sqrtf(networkNode.Capacity.value() / (boost::math::constants::two_pi<float>()));
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


void DrawLink(sf::RenderWindow& renderWindow, model::Node const& sourceNode, model::Node const& targetNode, model::Link const& networkLink)
{
  model::LinkGeometry linkGeometry = model::GetLinkGeometry(sourceNode, targetNode);

  model::Length const linkSideOffset = kLinkArrowWidth * model::meter;
  model::Position const linkSideDelta { Y(linkGeometry.direction) * linkSideOffset, -X(linkGeometry.direction) * linkSideOffset };

  model::Length const sourceNetworkNodeVisualRadius = GetNetworkNodeRadius(sourceNode) * model::meter;
  model::Length const targetNetworkNodeVisualRadius = GetNetworkNodeRadius(targetNode) * model::meter;
  model::Position const linkOrigin = sourceNode.Position + linkGeometry.direction * sourceNetworkNodeVisualRadius + linkSideDelta;
  model::Position const linkTarget = targetNode.Position - linkGeometry.direction * targetNetworkNodeVisualRadius + linkSideDelta;

  DrawLine(renderWindow, sf::Color::White, linkOrigin, linkTarget);
  DrawArrowEnd(renderWindow, linkTarget, linkGeometry.direction, kLinkArrowLength, kLinkArrowWidth);

  // Draw frames
  float const logicLinkToVisualLinkLength = (linkGeometry.length - sourceNetworkNodeVisualRadius - targetNetworkNodeVisualRadius) / linkGeometry.length;

  for (model::Frame const& frame : networkLink.FrameQueue)
  {
    model::Position const frameOrigin = linkOrigin + linkGeometry.direction * (fmax(0.0f, boost::units::quantity_cast<float>(frame.linkPos)) * logicLinkToVisualLinkLength * model::meter);
    model::Position const frameEnd    = linkOrigin + linkGeometry.direction * (fmin(boost::units::quantity_cast<float>(networkLink.GetLength()), boost::units::quantity_cast<float>(frame.linkPos + frame.length)) * logicLinkToVisualLinkLength * model::meter);

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

model::Position GetLinkMiddlePoint(model::Network const& network, model::Node const& sourceNode, model::Node const& targetNode, model::Link const& link)
{
  model::LinkGeometry linkGeometry = model::GetLinkGeometry(sourceNode, targetNode);

  model::Length const linkSideOffset = kLinkArrowWidth * model::meter;
  model::Position const linkSideDelta { Y(linkGeometry.direction) * linkSideOffset, -X(linkGeometry.direction) * linkSideOffset };
  
  return (sourceNode.Position + targetNode.Position) * 0.5f + linkSideDelta;
}

}