#include "DebugDrawWorld.h"
#include "DebugDrawAgent.h"
#include "DebugDrawNetwork.h"
#include "bitflow/model/World.h"
#include "bitflow/debug/DebugManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

namespace bitflow::debug
{

#if DEBUG_DRAW
void DrawWorld(model::World const& world)
{
  DrawNetwork(world.Network);
  DrawAgentVector(world.Network, world.Agents);
}
#endif // DEBUG_DRAW

} // namespace bitflow::debug