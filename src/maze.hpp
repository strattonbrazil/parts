#include <SFML/Graphics.hpp>

#include <memory>
#include <set>

class Cell
{
public:
    bool up;
    bool down;
    bool left;
    bool right;
};

class Maze : public sf::Drawable
{
public:
    Maze(int width, int height);
    ~Maze();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    Cell getCell(int row, int column) const;

    void setCursorPosition(sf::Vector2i pos);
    sf::Vector2i extendFromCursor(sf::Vector2i p) const;

    const int MAZE_WIDTH;
    const int MAZE_HEIGHT;
private:
    std::set<std::string> _openWalls;
    sf::Vector2i _cursorPosition;
};
