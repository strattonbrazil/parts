#ifndef UTIL_H
#define UTIL_H

#include <SFML/Graphics.hpp>

sf::Vector2i toI(sf::Vector2f v);
sf::Vector2f toF(sf::Vector2i v);
sf::Vector2f normalize(sf::Vector2f a);
sf::Vector2f extendFromPoint(sf::Vector2f p1, sf::Vector2f p2);
float distance(sf::Vector2f a, sf::Vector2f b);

#endif
