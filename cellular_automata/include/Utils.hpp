#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

sf::Vector2f windowToWorldTransform(sf::Vector2f& windowPos, sf::RenderWindow& window, sf::View& view);

#endif
