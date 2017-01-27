#include "DebugDrawNetwork.h"
#include "DebugManager.h"
#include "bitflow/model/Network.h"
#include "bitflow/model/Node.h"
#include "bitflow/model/Link.h"
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace bitflow::debug
{

namespace // unnamed namespace (internals)
{

void DrawNode(model::Node const& networkNode)
{
  DebugManager::GetInstance().DrawText(networkNode.Position, fmt::format("{}", networkNode.Capacity), 12);
}

void DrawLink(model::Node const& sourceNetworkNode, model::Node const& targetNetworkNode, model::Link const& networkLink)
{
  DebugManager::GetInstance().DrawText((sourceNetworkNode.Position + targetNetworkNode.Position) * 0.5f, fmt::format("{}/{}", networkLink.GetBandwidth(), networkLink.GetLatency()), 12);
}

} // unnamed namespace (internals)

#if DEBUG_DRAW

void Draw(model::Network const& network)
{
  network.ForEachNetworkNode(DrawNode);
  network.ForEachNetworkLink(DrawLink);
}

#endif // DEBUG_DRAW

} // namespace bitflow::debug
