#include <SFML/Graphics.hpp>

#ifndef PLAYER_H
#define PLAYER_H

class Player : public sf::Drawable
{
public:
    Player();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Vector2f mazePos() const;
    sf::Vector2i screenPos() const;
    bool isMoving() { return _moving; }

    void update();
    void setDirection(sf::Vector2f dir) { _direction = dir; }
    void setMove(sf::Vector2i destination);

private:
    sf::Vector2f _direction;
    sf::Vector2i _from;
    sf::Vector2i _to;
    bool _moving;
    float _transition;
};

#endif
