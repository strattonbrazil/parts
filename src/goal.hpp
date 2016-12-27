#include <SFML/Graphics.hpp>

#ifndef GOAL_H
#define GOAL_H

class Goal : public sf::Drawable
{
public:
    Goal(const int ROW, const int COLUMN);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Vector2f mazePos() { return sf::Vector2f(_pos.x + 0.5f, _pos.y + 0.5f); }
private:
    sf::Vector2i _pos;
};

#endif
