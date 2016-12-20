#pragma once

#include "Types.h"
#include "Reflection.h"
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/support/pair.hpp>
#include <string>

namespace bitflow::model
{

struct NetworkNode
{
  Position Position;
  InfoAmount Capacity;
};

} // namespace bitflow::model

BOOST_FUSION_ADAPT_STRUCT(
  bitflow::model::NetworkNode,
  Position,
  Capacity
)


