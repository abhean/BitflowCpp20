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

  Network()
    : m_graph()
    , m_nodeMap(m_graph)
    , m_arcMap(m_graph)
  {
  }

  void AddNode(NetworkNode const& node)
  {
    lemon::ListDigraph::Node newNode = m_graph.addNode();
    m_nodeMap[newNode] = node;
  }

  void AddLink(std::string_view const& sourceNodeName, std::string_view const& targetNodeName, NetworkLink const& link)
  {
    std::optional<lemon::ListDigraph::Node> source = FindNode(sourceNodeName);
    std::optional<lemon::ListDigraph::Node> target = FindNode(targetNodeName);

    if (source.has_value() && target.has_value())
    {
      lemon::ListDigraph::Arc newArc = m_graph.addArc(source.value(), target.value());
      m_arcMap[newArc] = link;
    }
  }

  std::optional<lemon::ListDigraph::Node> FindNode(std::string_view const& id)
  {
    lemon::ListDigraph::Node node;
    for (m_graph.first(node); m_graph.valid(node); m_graph.next(node))
    {
      if (m_nodeMap[node].Id == id)
      {
        return std::make_optional(node);
      }
    }

    return std::optional<lemon::ListDigraph::Node>();
  }

  lemon::ListDigraph& GetGraph() { return m_graph;  }

private:

  lemon::ListDigraph m_graph;
  lemon::ListDigraph::NodeMap<NetworkNode> m_nodeMap;
  lemon::ListDigraph::ArcMap<NetworkLink> m_arcMap;
};

bool LoadNetworkFromGraphML(std::experimental::filesystem::path const& path, Network& network);


} } // namespace bitflow::model

