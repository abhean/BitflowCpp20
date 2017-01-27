#include "Types.h"
#include "Network.h"
#include "Node.h"

namespace bitflow::model {

///
void StartTransfer(Network& network, NetworkLinkHandle const& networkLinkHandle, Info const& infoAmount)
{
  Link& networkLink = network.GetLink(networkLinkHandle);

  Length const frameLength{infoAmount / networkLink.GetBandwidth() * networkLink.GetPropagationVelocity()};
  networkLink.FrameQueue.emplace_back(-frameLength, frameLength);
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
