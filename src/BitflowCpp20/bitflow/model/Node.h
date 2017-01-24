#pragma once

#include "Types.h"
#include "foundation/Reflection.h"
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/support/pair.hpp>
#include <string>

namespace bitflow::model
{

struct Node
{
  Position Position;
  Info Capacity;
};

} // namespace bitflow::model

BOOST_FUSION_ADAPT_STRUCT
(
  bitflow::model::Node,
  Position,
  Capacity
)


