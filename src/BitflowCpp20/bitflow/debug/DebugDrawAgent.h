#pragma once

#include "DebugConfig.h"
#include "bitflow/model/Agent.h"

// Forward declarations
namespace bitflow::model
{
  class Network;
} // namespace model

namespace sf
{
  class RenderWindow;
} // namespace sf

//
namespace bitflow::debug
{

#if DEBUG_DRAW
void DrawAgentVector(model::Network const& network, model::AgentVector const& agentVector);
#endif // DEBUG_DRAW

} // namespace bitflow::debug