#pragma once

#include "DebugConfig.h"

// Forward declarations
namespace model
{
  class World;
}

//
namespace bitflow::debug
{

#if DEBUG_DRAW
void Draw(model::World const& world);
#endif // DEBUG_DRAW

}