#pragma once

#include "Types.h"
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/support/pair.hpp>

namespace bitflow::model
{

struct NetworkLink
{
  Length Length;
  Time Latency;
  Bandwidth Bandwidth;
};

} // namespace bitflow::model

BOOST_FUSION_ADAPT_STRUCT(
  bitflow::model::NetworkLink,
  Length,
  //Latency,
  //Bandwidth
)
