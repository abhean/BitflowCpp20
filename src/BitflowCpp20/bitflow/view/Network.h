#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "bitflow/model/Network.h"

namespace bitflow::view
{

void Draw(sf::RenderWindow& renderWindow, model::Network const& network);

}