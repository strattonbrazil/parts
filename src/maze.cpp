#include <maze.hpp>

#include <SFML/OpenGL.hpp>

#include <set>
#include <tuple>

#include <boost/python.hpp>

#include <iostream>


Maze::Maze(int width, int height)
{
    Py_Initialize();

    // now time to insert the current working directory into the python path so module search can take advantage
    // this must happen after python has been initialised
    PyObject* sysPath = PySys_GetObject("path");
    PyList_Insert( sysPath, 0, PyString_FromString("/home/stratton/Public/parts/python"));

    boost::python::object main_module = boost::python::import("__main__");
    boost::python::object main_namespace = main_module.attr("__dict__");
    /*
    boost::python::object ignored = exec("hello = file('hello.txt', 'w')\n"
                          "hello.write('Hello world!')\n"
                          "hello.close()",
                          main_namespace);
                          */


    boost::python::object mazeLib = boost::python::import("maze");
    boost::python::object createMaze = mazeLib.attr("create_maze");
    createMaze(4,5);
    //exec("from maze import foo", main_namespace);
    //boost::python::object foo = eval("foo", main_namespace);
    //std::cout << boost::python::extract<double>(foo) << std::endl;

        /*
    boost::python::object rand_func = rand_mod.attr("random");
    boost::python::object rand2 = rand_func();
    std::cout << boost::python::extract<double>(rand2) << std::endl;
    */


      cells = new Cell[width*height];

    std::set<std::tuple<int,int>> path;
    path.insert(std::make_tuple(0,0));

}

Maze::~Maze()
{
    delete[] cells;
}

void Maze::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Vector2u viewport = target.getSize();
    glViewport(0, 0, viewport.x, viewport.y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, viewport.x, 0, viewport.y, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_TRIANGLES);
    {
        glColor3f(1,1,0);
        glVertex2f(20, 20);
        glVertex2f(200, 20);
        glVertex2f(200, 200);
    }
    glEnd();
}
