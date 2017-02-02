#include "DrawWorld.h"
#include "DrawNetwork.h"
#include "DrawAgent.h"
#include "bitflow/model/Agent.h"

namespace bitflow::draw
{

namespace // unnamed namespace (internals)
{
} // unnamed namespace (internals)

void DrawWorld(sf::RenderWindow& renderWindow, model::World const & world)
{
  DrawNetwork(renderWindow, world.Network);
  DrawAgentVector(renderWindow, world.Network, world.Agents);
}

}
