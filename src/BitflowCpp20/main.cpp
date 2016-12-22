#include "bitflow/model/Network.h"
#include "bitflow/model/Types.h"
#include "bitflow/view/Network.h"
#include <spdlog/spdlog.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>

int main(int const argc, char const** argv)
{
  auto console = spdlog::stdout_color_mt("console");

  bitflow::model::Network network;
  if (bitflow::model::LoadNetworkFromGraphML(std::experimental::filesystem::path("assets/test.graphml"), network))
  {
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    // run the program as long as the window is open
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

        // clear the window with black color
        window.clear(sf::Color::Black);

        bitflow::view::Draw(window, network);

        // end the current frame
        window.display();
    }
  }

  return 0;
}