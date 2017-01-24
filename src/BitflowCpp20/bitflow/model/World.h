#pragma once

#include "Network.h"
#include "Agent.h"

namespace bitflow::model
{

class World
{
public:

  Network Network;
  AgentVector Agents;
};

void Update(World& world);

}
