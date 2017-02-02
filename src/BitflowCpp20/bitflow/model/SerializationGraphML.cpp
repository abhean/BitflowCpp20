#include "SerializationGraphML.h"
#include "Network.h"
#include "bitflow/debug/DebugManager.h"
#include <spdlog/spdlog.h>
#include <boost/graph/graphml.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/fusion/support/pair.hpp>
#include <iostream>
#include <any>

using namespace std::string_literals;

namespace bitflow::model 
{

namespace // unnamed namespace (internals)
{

struct GraphAttributeDefinition
{
  GraphAttributeDefinition()
  {}

  GraphAttributeDefinition(std::string id, std::string name, std::string type, std::string default)
    : Id(id)
    , Name(name)
    , Type(type)
    , Default(default)
  {}

  std::string Id;
  std::string Name;
  std::string Type;
  std::string Default;
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

template <typename TGraphElem>
struct GraphElemTraits {};

template <>
struct GraphElemTraits<Network>
{
  static ElemType const ElemType = ElemType::Graph;
};

template <>
struct GraphElemTraits<LinkBuilder>
{
  static ElemType const ElemType = ElemType::Edge;
};

template <>
struct GraphElemTraits<Node>
{
  static ElemType const ElemType = ElemType::Node;
};

/// Read Graph Attribute Definitions.
/// @param[in] graphDocPropertyTree The GraphML property tree root
/// @return Graph attributes definitions
GraphAttributeDefinitions ReadGraphAttributeDefinitionsFromGraphML(boost::property_tree::ptree const& graphDocPropertyTree)
{
  auto& console = debug::DebugManager::GetInstance().GetConsole();

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
        boost::optional<std::string> optDefault = attrDef.get_optional<std::string>(boost::property_tree::path("default"s));
        std::string const& default = optDefault.value_or(""s);
        graphElemAttributeDefinitions.emplace_back(id, name, type, default);

        console->info("{}/{}={}::{}={}", attrFor, id, name, type, default);
      }
      else
      {
        // YFiles specific attribute?
        boost::optional<std::string> optYFilesType = defXmlAttributes.get_optional<std::string>(boost::property_tree::path("yfiles.type"s, '/'));
        if (optYFilesType.is_initialized())
        {
          std::string const& yFilesType = optYFilesType.value();
          graphElemAttributeDefinitions.emplace_back(id, ""s, yFilesType, ""s);
          
          console->info("{}/{}=::{}", attrFor, id, yFilesType);
        }
      }
    }
  }

  return graphAttributeDefinitions;
}

template <typename TGraphElement>
void ReadGraphElementGenericAttributeFromGraphML(GraphAttributeDefinition const& graphAttributeDefinition, boost::optional<boost::property_tree::ptree const&> optAttributeData, TGraphElement& graphElement_)
{
  std::string const& value = optAttributeData ? optAttributeData->data() : graphAttributeDefinition.Default;
  apply_fusion_visitor(SetMemberValueFromStrVisitor<Info>(graphAttributeDefinition.Name, value), graphElement_);
}

template <typename TGraphElement>
void ReadGraphElementAttributeFromGraphML(GraphAttributeDefinition const& graphAttributeDefinition, boost::optional<boost::property_tree::ptree const&> optAttributeData, TGraphElement& graphElement_)
{
  ReadGraphElementGenericAttributeFromGraphML(graphAttributeDefinition, optAttributeData, graphElement_);
}

template <>
void ReadGraphElementAttributeFromGraphML(GraphAttributeDefinition const& graphAttributeDefinition, boost::optional<boost::property_tree::ptree const&> optAttributeData, Node& graphNode_)
{
  if (graphAttributeDefinition.Type == "nodegraphics"s)
  {
    assert(optAttributeData);
    boost::property_tree::ptree const& geometryTreeXmlAttributes = optAttributeData->get_child("y:ShapeNode.y:Geometry.<xmlattr>"s);
    graphNode_.Position = Position{ geometryTreeXmlAttributes.get<float>("x"s) * meter, geometryTreeXmlAttributes.get<float>("y"s) * meter };
  }
  else
  {
    ReadGraphElementGenericAttributeFromGraphML(graphAttributeDefinition, optAttributeData, graphNode_);
  }
}

template <typename TGraphElement>
void ReadGraphElementDataFromGraphML(GraphAttributeDefinitions const& graphAttributeDefinitions, boost::property_tree::ptree const& graphElementData, TGraphElement& graphElement_)
{
  std::size_t const elemTypeIdx = GetElemTypeIdx(GraphElemTraits<TGraphElement>::ElemType);
  AttributeDefVector const& attributeDefVector = graphAttributeDefinitions[elemTypeIdx];

  auto datasRange = graphElementData.equal_range("data"s);

  for (GraphAttributeDefinition const& attributeDef : attributeDefVector)
  {
    boost::optional<boost::property_tree::ptree const&> attributeDataTree;

    for (auto dataIterator = datasRange.first; dataIterator != datasRange.second; ++dataIterator)
    {
      boost::property_tree::ptree const& dataTree = dataIterator->second;
      boost::property_tree::ptree const& dataTreeXmlAttributes = dataTree.get_child("<xmlattr>"s);
      std::string dataKey = dataTreeXmlAttributes.get<std::string>("key"s);
      if (dataKey == attributeDef.Id)
      {
        attributeDataTree = dataTree;
        break;
      }
    }

    ReadGraphElementAttributeFromGraphML(attributeDef, attributeDataTree, graphElement_);
  }
}

} // unnamed namespace (internals)

bool LoadFromGraphML(Network& network, std::experimental::filesystem::path const& path)
{
  auto& console = debug::DebugManager::GetInstance().GetConsole();

  try
  {
    std::ifstream graphStream(path);
    if (graphStream.is_open())
    {
      boost::property_tree::ptree graphDocPropertyTree;
      boost::property_tree::xml_parser::read_xml(graphStream, graphDocPropertyTree);

      GraphAttributeDefinitions graphAttributeDefinitions = ReadGraphAttributeDefinitionsFromGraphML(graphDocPropertyTree);

      boost::property_tree::ptree graphPropertyTree = graphDocPropertyTree.get_child("graphml.graph"s);

      // read nodes
      Network::GraphSerializationAccess::GraphNodePropertyMap<std::string> nodeIdMap(Network::GraphSerializationAccess::GetGraph(network));

      auto nodesRange = graphPropertyTree.equal_range("node"s);
      for (auto nodeIterator = nodesRange.first; nodeIterator != nodesRange.second; ++nodeIterator)
      {
        Node networkNode;
        
        boost::property_tree::ptree const& nodeTree = nodeIterator->second;
        boost::property_tree::ptree const& nodeTreeXmlAttributes = nodeTree.get_child("<xmlattr>"s);
        ReadGraphElementDataFromGraphML(graphAttributeDefinitions, nodeTree, networkNode);
      
        NetworkNodeHandle graphNode = network.AddNetworkNode(networkNode);

        nodeIdMap[graphNode] = nodeTreeXmlAttributes.get<std::string>("id"s);
      }
      
      // read edges
      auto edgesRange = graphPropertyTree.equal_range("edge"s);
      for (auto edgeIterator = edgesRange.first; edgeIterator != edgesRange.second; ++edgeIterator)
      {
        boost::property_tree::ptree const& edgeTree = edgeIterator->second;
        boost::property_tree::ptree const& edgeTreeXmlAttributes = edgeTree.get_child("<xmlattr>"s);
        std::string const& sourceGraphNodeId = edgeTreeXmlAttributes.get<std::string>("source"s);
        std::string const& targetGraphNodeId = edgeTreeXmlAttributes.get<std::string>("target"s);
        std::optional<NetworkNodeHandle> optSourceGraphNode = network.FindNodeHandle([&nodeIdMap, sourceGraphNodeId](NetworkNodeHandle const& graphNode) { return sourceGraphNodeId == nodeIdMap[graphNode]; });
        std::optional<NetworkNodeHandle> optTargetGraphNode = network.FindNodeHandle([&nodeIdMap, targetGraphNodeId](NetworkNodeHandle const& graphNode) { return targetGraphNodeId == nodeIdMap[graphNode]; });

        if (optSourceGraphNode.has_value() && optTargetGraphNode.has_value())
        {
          NetworkNodeHandle const& sourceGraphNode = optSourceGraphNode.value();
          NetworkNodeHandle const& targetGraphNode = optTargetGraphNode.value();

          LinkBuilder linkBuilder;
          linkBuilder.SourceNode = &network.GetNode(sourceGraphNode);
          linkBuilder.TargetNode = &network.GetNode(targetGraphNode);
          ReadGraphElementDataFromGraphML(graphAttributeDefinitions, edgeTree, linkBuilder);

          network.AddNetworkLink(sourceGraphNode, targetGraphNode, linkBuilder.Build());
        }
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

} // namespace bitflow::model
