#include "Types.h"
#include "Network.h"
#include "NetworkNode.h"
#include <iostream>
#include <any>
#include <spdlog/spdlog.h>
#include <boost/graph/graphml.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/fusion/support/pair.hpp>

using namespace std::string_literals;

namespace bitflow { namespace model {

namespace // unnamed namespace (internals)
{

struct GraphAttributeDefinition
{
  GraphAttributeDefinition()
  {}

  GraphAttributeDefinition(std::string id, std::string name, std::string type)
    : Id(id)
    , Name(name)
    , Type(type)
  {}

  std::string Id;
  std::string Name;
  std::string Type;
};

using AttributeDefVector = std::vector<GraphAttributeDefinition>;

std::optional<GraphAttributeDefinition const> FindGraphAttributeDefinition(AttributeDefVector const& attributeDefVector, std::string const& id)
{
  for (GraphAttributeDefinition const& attributeDef : attributeDefVector)
  {
    if (attributeDef.Id == id)
    {
      return std::optional<GraphAttributeDefinition const>(attributeDef);
    }
  }

  return std::optional<GraphAttributeDefinition const>();
}

enum class ElemType
{
  Graph,
  Node,
  Edge,
  //...
  Count,
  Invalid
};

constexpr std::size_t GetElemTypeIdx(ElemType const elemType)
{
  return static_cast<std::size_t>(elemType);
}

ElemType ElemTypeFromString(std::string_view const& elemTypeName)
{
  constexpr std::size_t const elemTypeCount = GetElemTypeIdx(ElemType::Count);
  static std::array<std::string const, elemTypeCount> const elemTypesNames =
  {
    "graph", "node", "edge"
  };

  for (int i = 0; i < elemTypeCount; ++i)
  {
    if (elemTypesNames[i] == elemTypeName)
    {
      return static_cast<ElemType>(i);
    }
  }

  return ElemType::Invalid;
}

inline std::istream& operator>>(std::istream& in, ElemType& elemType)
{
  std::string elemTypeName;
  in >> elemTypeName;
  elemType = ElemTypeFromString(elemTypeName);

  return in;
}

using GraphAttributeDefinitions = std::array<AttributeDefVector, GetElemTypeIdx(ElemType::Count)>;

/// Read Graph Attribute Definitions.
/// @param[in] graphDocPropertyTree The GraphML property tree root
/// @return Graph attributes definitions
GraphAttributeDefinitions ReadGraphAttributeDefinitionsFromGraphML(boost::property_tree::ptree const& graphDocPropertyTree)
{
  auto console = spdlog::get("console");

  GraphAttributeDefinitions graphAttributeDefinitions;

  boost::property_tree::ptree graphPropertyTree = graphDocPropertyTree.get_child("graphml"s);

  auto attributeDefinitionsRange = graphPropertyTree.equal_range("key"s);
  for (auto attrDefIterator = attributeDefinitionsRange.first; attrDefIterator != attributeDefinitionsRange.second; ++attrDefIterator)
  {
    boost::property_tree::ptree const& attrDef = attrDefIterator->second;

    boost::property_tree::ptree const& defXmlAttributes = attrDef.get_child("<xmlattr>"s);
    
    std::string attrFor = defXmlAttributes.get<std::string>("for"s);
    ElemType attrElemType = boost::lexical_cast<ElemType>(attrFor);
    if (attrElemType != ElemType::Invalid)
    {
      std::size_t attrElemTypeIdx = GetElemTypeIdx(attrElemType);
      AttributeDefVector& graphElemAttributeDefinitions = graphAttributeDefinitions[attrElemTypeIdx];

      std::string const& id = defXmlAttributes.get<std::string>("id");
      // GraphML standard attribute
      boost::optional<std::string> optName = defXmlAttributes.get_optional<std::string>(boost::property_tree::path("attr.name"s, '/'));
      if (optName.is_initialized())
      {
        std::string const& name = optName.value();
        std::string const& type = defXmlAttributes.get<std::string>(boost::property_tree::path("attr.type"s, '/'));
        graphElemAttributeDefinitions.emplace_back(id, name, type);

        console->info("{}/{}={}::{}", attrFor, id, name, type);
      }
      else
      {
        // YFiles specific attribute?
        boost::optional<std::string> optYFilesType = defXmlAttributes.get_optional<std::string>(boost::property_tree::path("yfiles.type"s, '/'));
        if (optYFilesType.is_initialized())
        {
          std::string const& yFilesType = optYFilesType.value();
          graphElemAttributeDefinitions.emplace_back(id, ""s, yFilesType);
          
          console->info("{}/{}=::{}", attrFor, id, yFilesType);
        }
      }
    }
  }

  return graphAttributeDefinitions;
}

template <typename TGraphElement>
void ReadGraphElementGenericAttributeFromGraphML(GraphAttributeDefinition const& graphAttributeDefinition, boost::property_tree::ptree const& attributeData, TGraphElement& graphElement_)
{
  apply_fusion_visitor(SetMemberValueFromStrVisitor<InfoAmount>(graphAttributeDefinition.Name, attributeData.data()), graphElement_);
}

template <typename TGraphElement>
void ReadGraphElementAttributeFromGraphML(GraphAttributeDefinition const& graphAttributeDefinition, boost::property_tree::ptree const& attributeData, TGraphElement& graphElement_)
{
  ReadGraphElementGenericAttributeFromGraphML(graphAttributeDefinition, attributeData, graphElement_);
}

template <>
void ReadGraphElementAttributeFromGraphML(GraphAttributeDefinition const& graphAttributeDefinition, boost::property_tree::ptree const& attributeData, NetworkNode& graphNode_)
{
  if (graphAttributeDefinition.Type == "nodegraphics"s)
  {
    boost::property_tree::ptree const& geometryTreeXmlAttributes = attributeData.get_child("y:ShapeNode.y:Geometry.<xmlattr>"s);
    graphNode_.Position = Position{ geometryTreeXmlAttributes.get<float>("x"s) * meter, geometryTreeXmlAttributes.get<float>("y"s) * meter };
  }
  else
  {
    ReadGraphElementGenericAttributeFromGraphML(graphAttributeDefinition, attributeData, graphNode_);
  }
}

template <typename TGraphElement>
void ReadGraphElementDataFromGraphML(GraphAttributeDefinitions const& graphAttributeDefinitions, boost::property_tree::ptree const& graphElementData, TGraphElement& graphElement_)
{
  auto datasRange = graphElementData.equal_range("data"s);
  for (auto dataIterator = datasRange.first; dataIterator != datasRange.second; ++dataIterator)
  {
    boost::property_tree::ptree const& dataTree = dataIterator->second;
    boost::property_tree::ptree const& dataTreeXmlAttributes = dataTree.get_child("<xmlattr>"s);
    std::string dataKey = dataTreeXmlAttributes.get<std::string>("key"s);

    std::optional<GraphAttributeDefinition const> optAttributeDefinition = FindGraphAttributeDefinition(graphAttributeDefinitions[GetElemTypeIdx(ElemType::Node)], dataKey);
    if (optAttributeDefinition.has_value())
    {
      GraphAttributeDefinition const& attributeDefinition = optAttributeDefinition.value();
      ReadGraphElementAttributeFromGraphML(attributeDefinition, dataTree, graphElement_);
    }
  }
}

} // unnamed namespace (internals)

bool LoadNetworkFromGraphML(std::experimental::filesystem::path const& path, Network& network)
{
  auto console = spdlog::get("console");

  try
  {
    std::ifstream graphStream(path);
    if (graphStream.is_open())
    {
      Network network;

      boost::property_tree::ptree graphDocPropertyTree;
      boost::property_tree::xml_parser::read_xml(graphStream, graphDocPropertyTree);

      GraphAttributeDefinitions graphAttributeDefinitions = ReadGraphAttributeDefinitionsFromGraphML(graphDocPropertyTree);

      boost::property_tree::ptree graphPropertyTree = graphDocPropertyTree.get_child("graphml.graph"s);

      // read nodes
      lemon::ListDigraph::NodeMap nodeIdMap(network.GetGraph());

      auto nodesRange = graphPropertyTree.equal_range("node"s);
      for (auto nodeIterator = nodesRange.first; nodeIterator != nodesRange.second; ++nodeIterator)
      {
        NetworkNode node;
        
        boost::property_tree::ptree const& nodeTree = nodeIterator->second;
        boost::property_tree::ptree const& nodeTreeXmlAttributes = nodeTree.get_child("<xmlattr>"s);
        node.Id = nodeTreeXmlAttributes.get<std::string>("id"s);
        ReadGraphElementDataFromGraphML(graphAttributeDefinitions, nodeTree, node);
      
        network.AddNode(node);
      }

      // read edges
      auto edgesRange = graphPropertyTree.equal_range("edge"s);
      for (auto edgeIterator = edgesRange.first; edgeIterator != edgesRange.second; ++edgeIterator)
      {
        NetworkLink link;
        
        boost::property_tree::ptree const& edgeTree = edgeIterator->second;
        boost::property_tree::ptree const& edgeTreeXmlAttributes = edgeTree.get_child("<xmlattr>"s);
        std::string sourceNode = edgeTreeXmlAttributes.get<std::string>("source"s);
        std::string targetNode = edgeTreeXmlAttributes.get<std::string>("target"s);

        ReadGraphElementDataFromGraphML(graphAttributeDefinitions, edgeTree, link);
      
        network.AddLink(sourceNode, targetNode, link);
      }
      return true;
    }
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return false;
}

} } // namespace bitflow::model
