#include "Agent.h"
#include "Network.h"
#include "bitflow/debug/DebugManager.h"

namespace bitflow::model
{

namespace // unnamed namespace (internals)
{

void TickState(model::Network& network, model::Agent& agent, model::Agent::Executing const& executingState)
{
  auto& console = debug::DebugManager::GetInstance().GetConsole();
  console->info("TickAgentInState executing");

  Node& agentNode = network.GetNode(executingState.NodeHandle);

  std::optional<NetworkLinkHandle> const& optNetworkLinkHandle = network.FindLinkHandle([&agentNode, &network](NetworkLinkHandle const& linkHandle) 
  {
    // TODO[egarcia]: retrieve source/target nodes
  });

  if (optNetworkLinkHandle)
  {
    // TODO[egarcia]: create transfer frame, etc.
    agent.state = Agent::BeingTransferred { *optNetworkLinkHandle };
  }
}

void TickState(model::Network& network, model::Agent& agent, model::Agent::BeingTransferred const& beingTransferredState)
{
  auto& console = debug::DebugManager::GetInstance().GetConsole();
  console->info("TickAgentInState being transferred");
}

} // unnamed namespace (internals)

void Tick(Network& network, AgentVector& agentVector, Time const timeDelta)
{
  for (model::Agent& agent : agentVector)
  {
    std::visit([&network, &agent](auto&& state) { TickState(network, agent, state);  }, agent.state);
  }
}

} // namespace bitflow::model
