#include <maze.hpp>
#include <constants.hpp>
#include <util.hpp>

#include <SFML/OpenGL.hpp>
#include <tuple>
#include <boost/python.hpp>

#include <iostream>

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

Maze::Maze(int width, int height) : MAZE_WIDTH(width), MAZE_HEIGHT(height)
{
    Py_Initialize();

    // now time to insert the current working directory into the python path so module search can take advantage
    // this must happen after python has been initialised
    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyList_Insert( sysPath, 0, PyString_FromString("/home/stratton/Public/parts/python"));

    boost::python::object main_module = boost::python::import("__main__");
    boost::python::object main_namespace = main_module.attr("__dict__");

    boost::python::object mazeLib = boost::python::import("maze");
    boost::python::object createMaze = mazeLib.attr("create_maze");
    boost::python::object maze = createMaze(MAZE_HEIGHT, MAZE_WIDTH);
    std::cout << "len of open walls: " << len(maze) << std::endl;
    for (int i = 0; i < len(maze); i++) {
        boost::python::object openWall(maze[i]);
        const int row = boost::python::extract<int>(boost::python::object(openWall[0]));
        const int column = boost::python::extract<int>(boost::python::object(openWall[1]));
        char const* c_str = boost::python::extract<char const*>(boost::python::object(openWall[2]));

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
    for (int row = 0; row < MAZE_HEIGHT; row++) {
        for (int column = 0; column < MAZE_WIDTH; column++) {
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
const int PADDING = 3;

void Maze::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Vector2u viewport = target.getSize();
    glViewport(0, 0, viewport.x, viewport.y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, viewport.x, viewport.y, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(MAZE_OFFSET, MAZE_OFFSET, 0);

    glLineWidth(3.0f);
    glBegin(GL_LINES);
    for (int row = 0; row < MAZE_HEIGHT; row++) {
        for (int column = 0; column < MAZE_WIDTH; column++) {
            Cell cell = getCell(row, column);
            if (!cell.up) {
                glColor3f(1,0,0);
                glVertex2f(column*CELL_OFFSET, row*CELL_OFFSET + PADDING);
                glVertex2f((column+1)*CELL_OFFSET, row*CELL_OFFSET + PADDING);
            }
            if (!cell.down) {
                glColor3f(1,1,1);
                glVertex2f(column*CELL_OFFSET, (row+1)*CELL_OFFSET - PADDING);
                glVertex2f((column+1)*CELL_OFFSET, (row+1)*CELL_OFFSET - PADDING);
            }
            if (!cell.left) {
                glColor3f(0,1,1);
                glVertex2f(column*CELL_OFFSET + PADDING, row*CELL_OFFSET);
                glVertex2f(column*CELL_OFFSET + PADDING, (row+1)*CELL_OFFSET);
            }
            if (!cell.right) {
                glColor3f(1,0,1);
                glVertex2f(column*CELL_OFFSET+CELL_OFFSET - PADDING, row*CELL_OFFSET);
                glVertex2f(column*CELL_OFFSET+CELL_OFFSET - PADDING, (row+1)*CELL_OFFSET);
            }
        }
    }
    glEnd();

    // draw stencil
#if 1
    sf::Vector2f correctedPosition = toF(_playerPosition - sf::Vector2i(MAZE_OFFSET, MAZE_OFFSET));
    glBegin(GL_QUADS);
    {
        glColor3f(1,0,0);
        for (int row = 0; row < MAZE_HEIGHT; row++) {
            for (int column = 0; column < MAZE_WIDTH; column++) {
                Cell cell = getCell(row, column);
                if (!cell.up) {
                    sf::Vector2f a(column*CELL_OFFSET, row*CELL_OFFSET);
                    sf::Vector2f b((column+1)*CELL_OFFSET, row*CELL_OFFSET);
                    sf::Vector2f aV = extendFromPoint(correctedPosition, a);
                    sf::Vector2f bV = extendFromPoint(correctedPosition, b);
                    drawQuad(a, b, bV, aV);
                }
                if (!cell.down) {
                    sf::Vector2f a(column*CELL_OFFSET, (row+1)*CELL_OFFSET);
                    sf::Vector2f b((column+1)*CELL_OFFSET, (row+1)*CELL_OFFSET);
                    sf::Vector2f aV = extendFromPoint(correctedPosition, a);
                    sf::Vector2f bV = extendFromPoint(correctedPosition, b);
                    drawQuad(a, b, bV, aV);
                }
                if (!cell.left) {
                    sf::Vector2f a(column*CELL_OFFSET, row*CELL_OFFSET);
                    sf::Vector2f b(column*CELL_OFFSET, (row+1)*CELL_OFFSET);
                    sf::Vector2f aV = extendFromPoint(correctedPosition, a);
                    sf::Vector2f bV = extendFromPoint(correctedPosition, b);
                    drawQuad(a, b, bV, aV);
                }
                if (!cell.right) {
                    sf::Vector2f a(column*CELL_OFFSET+CELL_OFFSET, row*CELL_OFFSET);
                    sf::Vector2f b(column*CELL_OFFSET+CELL_OFFSET, (row+1)*CELL_OFFSET);
                    sf::Vector2f aV = extendFromPoint(correctedPosition, a);
                    sf::Vector2f bV = extendFromPoint(correctedPosition, b);
                    drawQuad(a, b, bV, aV);
                }
            }
        }
    }
    glEnd();
#endif

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(_cursorPosition.x, _cursorPosition.y, 0);

    // draw cursor
    glColor3f(1,0,0);
    glBegin(GL_QUADS);
    {
        glVertex2f(-5,-5);
        glVertex2f(5,-5);
        glVertex2f(5,5);
        glVertex2f(-5,5);
    }
    glEnd();
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

void Maze::setPlayerPosition(sf::Vector2i pos)
{
    _playerPosition = pos;
}

void Maze::setCursorPosition(sf::Vector2i pos)
{
    _cursorPosition = pos;
}
