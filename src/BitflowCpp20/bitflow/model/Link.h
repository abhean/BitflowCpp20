#pragma once

#include "Frame.h"
#include "NetworkDefs.h"
#include "Types.h"
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/support/pair.hpp>

namespace bitflow::model
{
  struct Node;
} // namespace bitflow::model

namespace bitflow::model
{

class Link
{
public:
  
  Link()
    : m_Bandwidth()
    , m_PropagationVelocity()
    , m_Length()
    , m_Latency()
  {}

  Link(Bandwidth const& bandwidth, Velocity const& propagationVelocity, Length const& length, Time const& latency)
    : m_Bandwidth(bandwidth)
    , m_PropagationVelocity(propagationVelocity)
    , m_Length(length)
    , m_Latency(latency)
  {}

  // Config
  Bandwidth const& GetBandwidth() const { return m_Bandwidth; }
  Velocity const& GetPropagationVelocity() const { return m_PropagationVelocity; }
  Length const& GetLength() const { return m_Length; }
  Time const& GetLatency() const { return m_Latency; }

  // State
  FrameQueue FrameQueue;

private:

  // Configuration
  Velocity m_PropagationVelocity;
  Bandwidth m_Bandwidth;

  Length m_Length;
  // Cached values
  Time m_Latency;

};

struct LinkBuilder
{
  Bandwidth Bandwidth;
  Velocity PropagationVelocity;
  Node* SourceNode;
  Node* TargetNode;

  Link Build();
};


void Tick(Link& networkLink, Time timeDelta);

} // namespace bitflow::model

BOOST_FUSION_ADAPT_STRUCT
(
  bitflow::model::LinkBuilder,
  Bandwidth,
  PropagationVelocity
)
