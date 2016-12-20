#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <boost/python.hpp>

#include <player.hpp>
#include <maze.hpp>
#include <goal.hpp>
#include <util.hpp>

#include <iostream>

int main()
{
    Py_Initialize();

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("/tmp/kitten.jpg"))
       return EXIT_FAILURE;
    sf::Sprite sprite(texture);
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("./fonts/RobotoCondensed-Regular.ttf"))
       return EXIT_FAILURE;
    sf::Text text("Hello SFML", font, 50);

    Player player;
    const int NUM_ROWS = 7;
    const int NUM_COLUMNS = 10;
    Maze maze(NUM_ROWS,NUM_COLUMNS);
    Goal goal(NUM_ROWS-1, NUM_COLUMNS-1);

    sf::Clock clock;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32)); //"SFML window");

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();
        // Draw the sprite
        //window.draw(sprite);

        text.setPosition(100,100);
        text.setColor(sf::Color::Red);

        if (!player.isMoving()) {
            sf::Vector2f pos = player.mazePos();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !maze.hasWall(pos.x, pos.y, "down")) {
                player.setMove(sf::Vector2i(1,0));
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !maze.hasWall(pos.x, pos.y, "up")) {
                player.setMove(sf::Vector2i(-1,0));
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !maze.hasWall(pos.x, pos.y, "left")) {
                player.setMove(sf::Vector2i(0,-1));
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !maze.hasWall(pos.x, pos.y, "right")) {
                player.setMove(sf::Vector2i(0,1));
            }
        }

        float elapsed = clock.restart().asSeconds();
        float fps = 1.f / elapsed;
        //std::cout << "fps: " << fps << std::endl;

        player.update(elapsed);

        sf::Vector2i globalPosition = sf::Mouse::getPosition();

        // get the local mouse position (relative to a window)
        sf::Vector2i localPosition = sf::Mouse::getPosition(window);

        maze.setPlayerPosition(player.mazePos());


        sf::Vector2u windowSize = window.getSize();
        float aspect = (float)windowSize.x / windowSize.y;
        const float CELL_HEIGHT = NUM_ROWS + 2; // leave cell worth on top and bottom
        const float PIXELS_PER_CELL = windowSize.y / CELL_HEIGHT;
        const float NUM_HORIZONTAL_CELLS = windowSize.x / PIXELS_PER_CELL;

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, aspect * CELL_HEIGHT, CELL_HEIGHT, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef((NUM_HORIZONTAL_CELLS - NUM_COLUMNS) * 0.5f, 1, 0);
        glPushMatrix();
        window.draw(player);
        glPopMatrix();
        glPushMatrix();
        window.draw(maze);
        glPopMatrix();

        glPushMatrix();
        window.draw(goal);
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        // Draw the string
        //window.draw(text);

        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}
