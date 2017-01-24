#include "bitflow/model/World.h"
#include "bitflow/model/Types.h"
#include "bitflow/view/WorldView.h"
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
  auto console = spdlog::stdout_color_mt("console");

  bitflow::model::World world;
  if (bitflow::model::LoadFromGraphML(world.Network, std::experimental::filesystem::path("assets/test.graphml")))
  {
    // TEST!!
    std::optional<bitflow::model::NetworkLinkHandle> optLinkHandle = world.Network.FindLinkHandle([](bitflow::model::NetworkLinkHandle const&) { return true; });
    assert(optLinkHandle.has_value());
    bitflow::model::StartTransfer(world.Network, optLinkHandle.value(), 10000000.0f * bitflow::model::byte);
    // TEST!!

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    window.setFramerateLimit(60u);
    //window.setVerticalSyncEnabled(true);

    // run the program as long as the window is open
    sf::Clock Clock;

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Tick simulation
        sf::Time sfmlTime = Clock.restart();
        Tick(world.Network, sfmlTime.asSeconds() * bitflow::model::second);

        // clear the window with black color
        window.clear(sf::Color::Black);

        bitflow::view::Draw(window, world);

        // end the current frame
        window.display();
    }
  }

  return 0;
}