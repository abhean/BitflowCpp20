// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NetworkNode.h"
#include "NetworkLink.h"
#include <lemon/list_graph.h>
#include <lemon/maps.h>
#include <optional>
#include <experimental/filesystem>

namespace bitflow { namespace model {

class Network
{
public:

  using GraphNode = lemon::ListDigraph::Node;
  using GraphArc = lemon::ListDigraph::Arc;

  Network()
    : m_graph()
    , m_nodeMap(m_graph)
    , m_arcMap(m_graph)
  {
  }

  GraphNode AddNetworkNode(NetworkNode const& node)
  {
    GraphNode newNode = m_graph.addNode();
    m_nodeMap[newNode] = node;

    return newNode;
  }

  GraphArc AddNetworkLink(GraphNode const& sourceNode, GraphNode const& targetNode, NetworkLink const& link)
  {
    GraphArc newArc = m_graph.addArc(sourceNode, targetNode);
    m_arcMap[newArc] = link;

    return newArc;
  }

  template <typename Function>
  void ForEachNetworkNode(Function const& function)
  {
    lemon::ListDigraph::Node node;
    for (m_graph.first(node); m_graph.valid(node); m_graph.next(node))
    {
      function(m_nodeMap[node]);
    }
  }

  template <typename Function>
  void ForEachNetworkNode(Function const& function) const
  {
    GraphNode graphNode;
    for (m_graph.first(graphNode); m_graph.valid(graphNode); m_graph.next(graphNode))
    {
      function(m_nodeMap[graphNode]);
    }
  }

  template <typename function>
  void ForEachNetworkLink(function const& function) const
  {
    GraphArc graphArc;
    for (m_graph.first(graphArc); m_graph.valid(graphArc); m_graph.next(graphArc))
    {
      function(m_nodeMap[m_graph.source(graphArc)], m_nodeMap[m_graph.target(graphArc)], m_arcMap[graphArc]);
    }
  }

  // graph representation functions
  lemon::ListDigraph& GetGraph() { return m_graph;  }

  template <typename Predicate>
  std::optional<GraphNode> FindGraphNode(Predicate const& predicate) const
  {
    lemon::ListDigraph::Node node;
    for (m_graph.first(node); m_graph.valid(node); m_graph.next(node))
    {
      if (predicate(node))
      {
        return std::make_optional(node);
      }
    }

    return std::optional<GraphNode>();
  }


private:

  lemon::ListDigraph m_graph;
  lemon::ListDigraph::NodeMap<NetworkNode> m_nodeMap;
  lemon::ListDigraph::ArcMap<NetworkLink> m_arcMap;
};

bool LoadNetworkFromGraphML(std::experimental::filesystem::path const& path, Network& network);


} } // namespace bitflow::model

