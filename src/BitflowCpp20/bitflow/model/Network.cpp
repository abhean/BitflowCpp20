#include "Types.h"
#include "Network.h"
#include "Node.h"

namespace bitflow::model {

///
void StartTransfer(Network& network, NetworkLinkHandle const& networkLinkHandle, Info const& infoAmount)
{
  Link& networkLink = network.GetLink(networkLinkHandle);

  Frame const frame = [infoAmount, &networkLink = static_cast<Link const&>(networkLink)] {
    Length const frameLength{infoAmount / networkLink.GetBandwidth() * networkLink.GetPropagationVelocity()};
    return Frame{ -frameLength, frameLength };
  }();

  networkLink.FrameQueue.push_back(frame);
}

///
void Tick(Network& network, Time const timeDelta)
{
  network.ForEachNetworkLink([timeDelta](Node&, Node&, Link& link)
  {
    Tick(link, timeDelta);
  });
}

} // namespace bitflow::model
