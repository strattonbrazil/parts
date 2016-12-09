#include <SFML/Graphics.hpp>

#include <memory>

class Cell
{
public:
    Cell() : top(true), bottom(true), left(true), right(true) {}

    bool top;
    bool bottom;
    bool left;
    bool right;
};

class Maze : public sf::Drawable
{
public:
    Maze(int width, int height);
    ~Maze();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    Cell* cells;
};
