// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Node.h"
#include "Link.h"
#include "NetworkDefs.h"
#include "SerializationGraphML.h"
#include <lemon/list_graph.h>
#include <lemon/maps.h>
#include <optional>
#include <experimental/filesystem>

namespace bitflow::model
{

class Network
{
private:

  using Graph = lemon::ListDigraph;
  template <typename T> using GraphNodePropertyMap = lemon::ListDigraph::NodeMap<T>;
  template <typename T> using GraphArcPropertyMap = lemon::ListDigraph::ArcMap<T>;
  using GraphNodeMap = GraphNodePropertyMap<Node>;
  using GraphLinkMap = GraphArcPropertyMap<Link>;

public:

  class GraphSerializationAccess
  {
    // Published functionality
    using Graph = Network::Graph;
    template <typename T> using GraphNodePropertyMap = Network::GraphNodePropertyMap<T>;

    static Graph& GetGraph(Network& network) { return network.m_graph;  }

    // Allowed to access network graph
    friend bool LoadFromGraphML(Network& network, std::experimental::filesystem::path const& path);
  };

public:

  Network()
    : m_graph()
    , m_nodeMap(m_graph)
    , m_linkMap(m_graph)
  {
  }

  NetworkNodeHandle AddNetworkNode(Node const& node)
  {
    NetworkNodeHandle newNode = m_graph.addNode();
    m_nodeMap[newNode] = node;

    return newNode;
  }

  NetworkLinkHandle AddNetworkLink(NetworkNodeHandle const& sourceNode, NetworkNodeHandle const& targetNode, Link const& link)
  {
    NetworkLinkHandle newArc = m_graph.addArc(sourceNode, targetNode);
    m_linkMap[newArc] = link;

    return newArc;
  }

  template <typename Function>
  void ForEachNetworkNode(Function& function) const
  {
    NetworkNodeHandle graphNode;
    for (m_graph.first(graphNode); m_graph.valid(graphNode); m_graph.next(graphNode))
    {
      function(m_nodeMap[graphNode]);
    }
  }

  template <typename Function>
  void ForEachNetworkNode(Function& function)
  {
    lemon::ListDigraph::Node node;
    for (m_graph.first(node); m_graph.valid(node); m_graph.next(node))
    {
      function(m_nodeMap[node]);
    }
  }

  template <typename Function>
  void ForEachNetworkLink(Function& function) const
  {
    NetworkLinkHandle graphArc;
    for (m_graph.first(graphArc); m_graph.valid(graphArc); m_graph.next(graphArc))
    {
      function(m_nodeMap[m_graph.source(graphArc)], m_nodeMap[m_graph.target(graphArc)], m_linkMap[graphArc]);
    }
  }

  template <typename Function>
  void ForEachNetworkLink(Function& function)
  {
    NetworkLinkHandle graphArc;
    for (m_graph.first(graphArc); m_graph.valid(graphArc); m_graph.next(graphArc))
    {
      function(m_nodeMap[m_graph.source(graphArc)], m_nodeMap[m_graph.target(graphArc)], m_linkMap[graphArc]);
    }
  }

  Node& GetNode(NetworkNodeHandle const& networkNodeHandle)
  {
    return m_nodeMap[networkNodeHandle];
  }

  Node const& GetNode(NetworkNodeHandle const& networkNodeHandle) const
  {
    return m_nodeMap[networkNodeHandle];
  }

  Link& GetLink(NetworkLinkHandle const& networkLinkHandle)
  {
    return m_linkMap[networkLinkHandle];
  }

  Link const& GetLink(NetworkLinkHandle const& networkLinkHandle) const
  {
    return m_linkMap[networkLinkHandle];
  }

  // graph representation functions
  template <typename Predicate>
  std::optional<NetworkNodeHandle> FindNodeHandle(Predicate const& predicate) const
  {
    lemon::ListDigraph::Node node;
    for (m_graph.first(node); m_graph.valid(node); m_graph.next(node))
    {
      if (predicate(node))
      {
        return std::make_optional(node);
      }
    }

    return std::optional<NetworkNodeHandle>();
  }


  template <typename Predicate>
  std::optional<NetworkLinkHandle> FindLinkHandle(Predicate const& predicate) const
  {
    NetworkLinkHandle linkHandle;
    for (m_graph.first(linkHandle); m_graph.valid(linkHandle); m_graph.next(linkHandle))
    {
      if (predicate(linkHandle))
      {
        return std::make_optional(linkHandle);
      }
    }

    return std::optional<NetworkLinkHandle>();
  }

private:

  Graph m_graph;
  GraphNodeMap m_nodeMap;
  GraphLinkMap m_linkMap;
};


void StartTransfer(Network& network, NetworkLinkHandle const& networkLinkHandle, Info const& infoAmount);
void Tick(Network& network, Time timeDelta);

} // namespace bitflow::model

