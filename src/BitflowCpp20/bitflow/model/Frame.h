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
  Frame()
    : linkPos()
    , length()
  {}

  Frame(Length const& _linkPos, Length const& _length)
    : linkPos(_linkPos)
    , length(_length)
  {}

  Length linkPos;
  Length length;
};

using FrameQueue = std::list<Frame>;

} // namespace bitflow::model
