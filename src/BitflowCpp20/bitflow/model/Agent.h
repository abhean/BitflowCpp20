#pragma once

#include "Network.h"

namespace bitflow::model 
{

struct Agent
{
  NetworkNodeHandle Node;
  Info Size;
};
 
using AgentVector = std::vector<Agent>;

///
void Update(AgentVector& agentVector);

}