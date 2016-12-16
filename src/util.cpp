#include <util.hpp>

sf::Vector2i toI(sf::Vector2f v)
{
    return sf::Vector2i(v.x, v.y);
}

sf::Vector2f toF(sf::Vector2i v)
{
    return sf::Vector2f(v.x, v.y);
}

sf::Vector2f normalize(sf::Vector2f a)
{
    float invLen = 1.0f / sqrt((a.x * a.x) + (a.y * a.y));
    return sf::Vector2f(a.x * invLen, a.y * invLen);
}

sf::Vector2f extendFromPoint(sf::Vector2f p1, sf::Vector2f p2)
{
    sf::Vector2f v = normalize(p2 - p1);
    v *= 5000.0f; // amount to extend
    return v + p1;
}
