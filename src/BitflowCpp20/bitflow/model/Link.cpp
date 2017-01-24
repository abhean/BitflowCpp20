#include "Link.h"
#include "Node.h"

namespace bitflow::model
{

void Tick(Link& networkLink, Time const timeDelta)
{
  FrameQueue::iterator itFrame = networkLink.FrameQueue.begin();
  while (itFrame != networkLink.FrameQueue.end())
  {
    Frame& frame = *itFrame;

    Length const nextLinkPos = frame.LinkPos + timeDelta * networkLink.GetPropagationVelocity();
    if (nextLinkPos >= networkLink.GetLength())
    {
      itFrame = networkLink.FrameQueue.erase(itFrame);
      //FinishTransfer(frame);
    }
    else
    {
      frame.LinkPos = nextLinkPos;
      
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



