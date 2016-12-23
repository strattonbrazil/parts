#include <SFML/Graphics.hpp>
#include <boost/python.hpp>

#ifndef MINIGAME_H
#define MINIGAME_H

using namespace boost::python;

class MiniGame : public sf::Drawable
{
public:
    MiniGame();
    void update(const float elapsed);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    object _minigameUpdateFunc;
    object _gameContext;
};

#endif
