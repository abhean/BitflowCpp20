#pragma once

#include <lemon/list_graph.h>

namespace bitflow::model
{

using NetworkNodeHandle = lemon::ListDigraph::Node;
using NetworkLinkHandle = lemon::ListDigraph::Arc;

} // namespace bitflow::model
