#pragma once

#include "NetworkDefs.h"
#include "Types.h"
#include <variant>

namespace bitflow::model 
{

struct Agent
{
  struct InNode
  {
    NetworkNodeHandle NodeHandle;
  };

  struct InLink
  {
    NetworkLinkHandle LinkHandle;
  };

  using Location = std::variant<InNode, InLink>;

  Agent(Info const& _size, Location const& _location)
    : size(_size)
    , location(_location)
  { }

  Agent()
    : size()
    , location()
  {}

  Info size;
  Location location;
};
 
using AgentVector = std::vector<Agent>;

///
void Update(AgentVector& agentVector);

}