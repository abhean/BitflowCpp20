#include "DrawUtils.h"
#include <boost/math/constants/constants.hpp>

namespace bitflow::draw 
{

// Auxiliary functions
float GetInfoAmountVisualRadius(model::Info const& info)
{
  float const nodeSurfaceToVisualRadius = 10.0f;
  return nodeSurfaceToVisualRadius * sqrtf(boost::units::quantity_cast<float>(info) / (boost::math::constants::two_pi<float>()));
}

}
