#pragma once

#include <experimental/filesystem>

// Forward declarations
namespace bitflow::model
{
  class Network;
} // namespace bitflow::model

namespace bitflow::model
{

bool LoadFromGraphML(Network& network, std::experimental::filesystem::path const& path);

} // namespace // bitflow::model
