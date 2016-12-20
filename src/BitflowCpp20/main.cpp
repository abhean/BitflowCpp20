#include "Network.h"
#include "Types.h"
#include <spdlog/spdlog.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>

int main(int const argc, char const** argv)
{
  auto console = spdlog::stdout_color_mt("console");

  bitflow::model::Network network;
  if (bitflow::model::LoadNetworkFromGraphML(std::experimental::filesystem::path("assets/test.graphml"), network))
  {

  }

  char input;
  std::cin >> input;

  return 0;
}