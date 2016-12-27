#include <SFML/Graphics.hpp>
#include <boost/python.hpp>

#ifndef MINIGAME_H
#define MINIGAME_H

using namespace boost::python;

class MiniGame : public sf::Drawable
{
public:
    MiniGame();
    void update(const float elapsed, sf::Vector2f mousePos);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool isFinished() { return _finished; }
private:
    bool _finished;
    object _minigameUpdateFunc;
    object _gameContext;
};

#endif
