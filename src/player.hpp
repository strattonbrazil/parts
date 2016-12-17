#include <SFML/Graphics.hpp>

#ifndef PLAYER_H
#define PLAYER_H

class Player : public sf::Drawable
{
public:
    Player();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Vector2f mazePos() const;
    bool isMoving() { return _moving; }

    void update();
    void setMove(sf::Vector2i destination);

private:
    sf::Vector2i _from;
    sf::Vector2i _to;
    bool _moving;
    float _transition;
};

#endif
