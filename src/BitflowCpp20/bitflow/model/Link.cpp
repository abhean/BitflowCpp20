#include "Link.h"
#include "Node.h"

namespace bitflow::model
{

LinkGeometry GetLinkGeometry(model::Node const& sourceNode, model::Node const& targetNode)
{
  LinkGeometry linkGeometry;

  model::Position const linkDelta  = targetNode.Position - sourceNode.Position;

  linkGeometry.length = mag(linkDelta);
  assert(linkGeometry.length.value() > 0.0f);

  linkGeometry.direction = linkDelta / linkGeometry.length;

  return linkGeometry;
}

void Tick(Link& networkLink, Time const timeDelta)
{
  FrameQueue::iterator itFrame = networkLink.FrameQueue.begin();
  while (itFrame != networkLink.FrameQueue.end())
  {
    Frame& frame = *itFrame;

    Length const nextLinkPos = frame.linkPos + timeDelta * networkLink.GetPropagationVelocity();
    if (nextLinkPos >= networkLink.GetLength())
    {
      itFrame = networkLink.FrameQueue.erase(itFrame);
      //FinishTransfer(frame);
    }
    else
    {
      frame.linkPos = nextLinkPos;
      
      if (nextLinkPos < 0.0f * meter)
      {
        break;
      }
      else
      {
        ++itFrame;
      }
    }
  }
}


Link LinkBuilder::Build()
{
  assert(SourceNode && TargetNode);

  Length const length = mag(TargetNode->Position - SourceNode->Position);
  Time const latency = length / PropagationVelocity;

  return Link{ Bandwidth, PropagationVelocity, length, latency };
}

} // namespace bitflow::model



