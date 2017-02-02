#include "World.h"

namespace bitflow::model {

void Tick(World& world, Time timeDelta)
{
  Tick(world.Network, timeDelta);
  Tick(world.Network, world.Agents, timeDelta);
}

}