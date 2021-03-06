#pragma once

#include "DebugConfig.h"

// Forward declarations
namespace model
{
  class Network;
}

//
namespace bitflow::debug
{

#if DEBUG_DRAW
void DrawNetwork(model::Network const& network);
#endif // DEBUG_DRAW

}