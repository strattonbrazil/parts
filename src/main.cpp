#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <boost/python.hpp>

#include <player.hpp>
#include <maze.hpp>
#include <goal.hpp>
#include <util.hpp>
#include <minigame.hpp>

#include <iostream>

enum GameMode { SEARCHING, GOAL, FLEEING };

int main()
{
    Py_Initialize();
    // now time to insert the current working directory into the python path so module search can take advantage
    // this must happen after python has been initialised
    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyList_Insert( sysPath, 0, PyString_FromString("/home/stratton/Public/parts/python"));

    GameMode mode = GameMode::SEARCHING;

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
    const int NUM_ROWS = 1;
    const int NUM_COLUMNS = 3;
    Maze maze(NUM_ROWS,NUM_COLUMNS);
    Goal goal(NUM_ROWS-1, NUM_COLUMNS-1);

    sf::Clock clock;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1200, 900), "OpenGL", sf::Style::Default, sf::ContextSettings(32)); //"SFML window");

    sf::RenderTexture *minigameBuffer = 0;

    MiniGame miniGame;
    sf::Texture minigameTexture;

    float minigameOpacity;

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

        if (mode != GameMode::GOAL && !player.isMoving()) {
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


        sf::Vector2u windowSize = window.getSize();
        const float MINIGAME_PADDING = 40;
        const float MINIGAME_RENDER_SIZE = windowSize.y - MINIGAME_PADDING * 2;
        const float MINIGAME_LEFT_OFFSET = (windowSize.x - MINIGAME_RENDER_SIZE) * 0.5f;
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        sf::Vector2f minigameRelativePos(pos.x - MINIGAME_LEFT_OFFSET, pos.y - MINIGAME_PADDING);
        sf::Vector2f minigameNormalPos = minigameRelativePos / MINIGAME_RENDER_SIZE;

        player.update(elapsed);
        miniGame.update(elapsed, minigameNormalPos);

        maze.setPlayerPosition(player.mazePos());

        // entering goal area
        if (mode == GameMode::SEARCHING && distance(player.mazePos(), goal.mazePos()) < 0.5f) {
            mode = GameMode::GOAL;
            minigameOpacity = 0.0f;
        } else if (mode == GameMode::GOAL && miniGame.isFinished()) {
            std::cout << "finished" << std::endl;
            mode = GameMode::FLEEING;
        }

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

        if (mode == GameMode::GOAL) {
            const int MINIGAME_TEXTURE_SIZE = 256;
            if (minigameBuffer == 0) {
                minigameBuffer = new sf::RenderTexture();
                minigameBuffer->create(MINIGAME_TEXTURE_SIZE, MINIGAME_TEXTURE_SIZE, false);
                minigameTexture = minigameBuffer->getTexture();
            }

            minigameBuffer->setActive(true);
            minigameBuffer->draw(miniGame);
            window.setActive(true);

            minigameOpacity = std::min(minigameOpacity + 1.0f * elapsed, 1.0f);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, windowSize.x, windowSize.y, 0, -1, 1);

            // darken non-minigame portion of screen
            const float BACKGROUND_OPACITY = 0.8f;
            glColor4f(0, 0, 0, minigameOpacity * BACKGROUND_OPACITY);
            glBegin(GL_QUADS);
            {
                glVertex2f(0, 0);
                glVertex2f(windowSize.x, 0);
                glVertex2f(windowSize.x, windowSize.y);
                glVertex2f(0, windowSize.y);
            }
            glEnd();

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(MINIGAME_LEFT_OFFSET, MINIGAME_PADDING, 0);
            sf::Sprite sprite(minigameBuffer->getTexture());
            sprite.setScale(MINIGAME_RENDER_SIZE / MINIGAME_TEXTURE_SIZE, MINIGAME_RENDER_SIZE / MINIGAME_TEXTURE_SIZE);
            sprite.setColor(sf::Color(255,255,255,255 * minigameOpacity));
            window.draw(sprite);
        }

        // Update the window
        window.display();
    }

    delete minigameBuffer;

    return EXIT_SUCCESS;
}
