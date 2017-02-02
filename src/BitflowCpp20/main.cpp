#include "bitflow/model/World.h"
#include "bitflow/model/Types.h"
#include "bitflow/draw/DrawManager.h"
#include "bitflow/draw/DrawWorld.h"
#include "bitflow/debug/DebugManager.h"
#include "bitflow/debug/DebugDrawWorld.h"
#include <spdlog/spdlog.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include <chrono>
#include <SFML/System/Clock.hpp>

int main(int const argc, char const** argv)
{
  // Singletons initialization
  bitflow::debug::DebugManager::Init();
  bitflow::draw::DrawManager::Init();

  //
  bitflow::model::World world;
  if (bitflow::model::LoadFromGraphML(world.Network, std::experimental::filesystem::path("assets/test.graphml")))
  {
    // TEST!!

    //std::optional<bitflow::model::NetworkLinkHandle> optLinkHandle = world.Network.FindLinkHandle([](bitflow::model::NetworkLinkHandle const&) { return true; });
    //assert(optLinkHandle);
    //bitflow::model::StartTransfer(world.Network, *optLinkHandle, bitflow::model::Info(10.0f * bitflow::model::megabytes));

    std::optional<bitflow::model::NetworkNodeHandle> optNodeHandle = world.Network.FindNodeHandle([](bitflow::model::NetworkNodeHandle const&) { return true; });
    assert(optNodeHandle);
    world.Agents.emplace_back(1.0f * bitflow::model::byte, bitflow::model::Agent::Executing{*optNodeHandle});
    // TEST!!


    // run the program as long as the window is open
    sf::Clock Clock;
    bool drawDebugEnabled = false;

    sf::RenderWindow& window = bitflow::draw::DrawManager::GetInstance().GetRenderWindow();
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
          switch (event.type)
          {
            // "close requested" event: we close the window
            case sf::Event::Closed:
            {
              window.close();
            }
            break;

            case sf::Event::KeyPressed:
            {
              switch (event.key.code)
              {
                case sf::Keyboard::D:
                {
                  drawDebugEnabled = !drawDebugEnabled;
                }
                break;
              }
            }
            break;
          }
        }

        // Tick simulation
        sf::Time sfmlTime = Clock.restart();
        Tick(world, sfmlTime.asSeconds() * bitflow::model::second);

        // clear the window with black color
        window.clear(sf::Color::Black);

        bitflow::draw::DrawWorld(window, world);

        if (drawDebugEnabled)
        {
          bitflow::debug::DrawWorld(world);
        }

        // end the current frame
        window.display();
    }
  }

  // Singletons destruction
  bitflow::draw::DrawManager::Done();
  bitflow::debug::DebugManager::Done();

  return 0;
}