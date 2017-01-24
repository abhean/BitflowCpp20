#include "WorldView.h"
#include "NetworkView.h"
#include "bitflow/model/Agent.h"

namespace bitflow::view
{

void bitflow::view::Draw(sf::RenderWindow& renderWindow, model::World const & world)
{
  Draw(renderWindow, world.Network);

  for (model::Agent const& agent : world.Agents)
  {


  }


}

}
