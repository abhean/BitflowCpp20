#pragma once

#include "NetworkDefs.h"
#include "Types.h"
#include <variant>

// Forward declarations
namespace bitflow::model
{
  class Network;
}

//
namespace bitflow::model 
{

struct Agent
{
  struct Executing
  {
    NetworkNodeHandle NodeHandle;
  };

  struct BeingTransferred
  {
    NetworkLinkHandle LinkHandle;
  };

  using State = std::variant<Executing, BeingTransferred>;

  Agent(Info const& _size, State const& _state)
    : size(_size)
    , state(_state)
  { }

  Agent()
    : size()
    , state()
  {}

  Info size;
  State state;
};
 
using AgentVector = std::vector<Agent>;

///
void Tick(Network& network, AgentVector& agentVector, Time timeDelta);

}