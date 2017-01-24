#pragma once

#include "Types.h"
#include <list>

// Forward declarations
namespace bitflow::model
{
class Link;
}

///
namespace bitflow::model
{

struct Frame
{
  Length LinkPos;
  Length Length;
};

using FrameQueue = std::list<Frame>;

} // namespace bitflow::model
