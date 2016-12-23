#include <maze.hpp>
#include <util.hpp>

#include <SFML/OpenGL.hpp>
#include <tuple>
#include <boost/python.hpp>

#include <iostream>

using namespace boost::python;

inline std::string wallToKey(int row, int column, std::string dir)
{
    return std::to_string(row) + "_" + std::to_string(column) + "_" + dir;
}

inline void drawQuad(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d)
{
    glVertex2f(a.x, a.y);
    glVertex2f(b.x, b.y);
    glVertex2f(c.x, c.y);
    glVertex2f(d.x, d.y);
}

inline void drawLine(sf::Vector2i a, sf::Vector2i b, float thickness = 2.0f)
{
    sf::Vector2f dir = normalize(toF(b - a));
    sf::Vector2f dirRotated(dir.y, -dir.x);

    // TODO: offset this beast to be centered on points
    glVertex2f(a.x, a.y);
    glVertex2f(a.x + dirRotated.x * thickness, a.y + dirRotated.y * thickness);
    glVertex2f(b.x + dirRotated.x * thickness, b.y + dirRotated.y * thickness);
    glVertex2f(b.x, b.y);
}

Maze::Maze(int numRows, int numColumns) : NUM_ROWS(numRows), NUM_COLUMNS(numColumns)
{
    object main_module = import("__main__");
    object main_namespace = main_module.attr("__dict__");

    object mazeLib = import("maze");
    object createMaze = mazeLib.attr("create_maze");
    object maze = createMaze(NUM_ROWS, NUM_COLUMNS);
    std::cout << "len of open walls: " << len(maze) << std::endl;
    for (int i = 0; i < len(maze); i++) {
        object openWall(maze[i]);
        const int row = extract<int>(object(openWall[0]));
        const int column = extract<int>(object(openWall[1]));
        char const* c_str = extract<char const*>(object(openWall[2]));

        std::string wallKey = wallToKey(row, column, c_str);
        _openWalls.insert(wallKey);
    }

    // print out open walls
    /*
    std::set<std::string>::iterator it;
    for (it = _openWalls.begin(); it != _openWalls.end(); ++it) {
        std::string wall = *it;
        std::cout << wall << std::endl;
    }
    std::cout << "----" << std::endl;
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int column = 0; column < NUM_COLUMNS; column++) {
            std::cout << "row: " << row << " column: " << column << std::endl;
            Cell cell = getCell(row,column);
            std::cout << "left: " << cell.left << std::endl;
            std::cout << "right: " << cell.right << std::endl;
            std::cout << "top: " << cell.up << std::endl;
            std::cout << "bottom: " << cell.down << std::endl;
        }
    }
    */
}

Maze::~Maze()
{
}

// for drawing maze
const float PADDING = 0.05f;

void Maze::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Vector2u viewport = target.getSize();
    glViewport(0, 0, viewport.x, viewport.y);

    glLineWidth(3.0f);
    glBegin(GL_LINES);
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int column = 0; column < NUM_COLUMNS; column++) {
            Cell cell = getCell(row, column);
            if (!cell.up) {
                glColor3f(1,0,0);
                glVertex2f(column, row + PADDING);
                glVertex2f(column+1, row + PADDING);
            }
            if (!cell.down) {
                glColor3f(1,1,1);
                glVertex2f(column, row+1 - PADDING);
                glVertex2f(column+1, row+1 - PADDING);
            }
            if (!cell.left) {
                glColor3f(0,1,1);
                glVertex2f(column + PADDING, row);
                glVertex2f(column + PADDING, row+1);
            }
            if (!cell.right) {
                glColor3f(1,0,1);
                glVertex2f(column+1 - PADDING, row);
                glVertex2f(column+1 - PADDING, row+1);
            }
        }
    }
    glEnd();

    // draw stencil
#if 0
    // player position in row/column form
    sf::Vector2f correctedPosition(_playerPosition.y, _playerPosition.x);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    {
        //glColor4f(1,0,0,0.1f);
        glColor4f(0,0,0,1);
        for (int row = 0; row < NUM_ROWS; row++) {
            for (int column = 0; column < NUM_COLUMNS; column++) {
                Cell cell = getCell(row, column);
                if (!cell.up) {
                    sf::Vector2f a(column, row);
                    sf::Vector2f b(column+1, row);
                    sf::Vector2f aV = extendFromPoint(correctedPosition, a);
                    sf::Vector2f bV = extendFromPoint(correctedPosition, b);
                    drawQuad(a, b, bV, aV);
                }
                if (!cell.down) {
                    sf::Vector2f a(column, row+1);
                    sf::Vector2f b(column+1, row+1);
                    sf::Vector2f aV = extendFromPoint(correctedPosition, a);
                    sf::Vector2f bV = extendFromPoint(correctedPosition, b);
                    drawQuad(a, b, bV, aV);
                }
                if (!cell.left) {
                    sf::Vector2f a(column, row);
                    sf::Vector2f b(column, row+1);
                    sf::Vector2f aV = extendFromPoint(correctedPosition, a);
                    sf::Vector2f bV = extendFromPoint(correctedPosition, b);
                    drawQuad(a, b, bV, aV);
                }
                if (!cell.right) {
                    sf::Vector2f a(column+1, row);
                    sf::Vector2f b(column+1, row+1);
                    sf::Vector2f aV = extendFromPoint(correctedPosition, a);
                    sf::Vector2f bV = extendFromPoint(correctedPosition, b);
                    drawQuad(a, b, bV, aV);
                }
            }
        }
    }
    glEnd();
    glDisable(GL_BLEND);
#endif
}

Cell Maze::getCell(int row, int column) const
{
    Cell cell;
    cell.left = _openWalls.find(wallToKey(row, column-1, "right")) != _openWalls.end();
    cell.right = _openWalls.find(wallToKey(row, column, "right")) != _openWalls.end();
    cell.up = _openWalls.find(wallToKey(row-1, column, "down")) != _openWalls.end();
    cell.down = _openWalls.find(wallToKey(row, column, "down")) != _openWalls.end();
    return cell;
}

bool Maze::hasWall(int row, int column, std::string dir)
{
    if (dir == "left") {
        dir = "right";
        column -= 1;
    } else if (dir == "up") {
        dir = "down";
        row -= 1;
    }
    return !(_openWalls.find(wallToKey(row, column, dir)) != _openWalls.end());
}

void Maze::setPlayerPosition(sf::Vector2f pos)
{
    _playerPosition = pos;
}

void Maze::setCursorPosition(sf::Vector2i pos)
{
    _cursorPosition = pos;
}
