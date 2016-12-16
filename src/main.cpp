#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <player.hpp>
#include <maze.hpp>
#include <util.hpp>

#include <iostream>

int main()
{
    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("/tmp/kitten.jpg"))
       return EXIT_FAILURE;
    sf::Sprite sprite(texture);
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("/tmp/OpenSans-Regular.ttf"))
       return EXIT_FAILURE;
    sf::Text text("Hello SFML", font, 50);

    Player player;
    Maze maze(9,9);

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

        // Draw the string
        //window.draw(text);

        if (!player.isMoving()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                player.setMove(sf::Vector2i(1,0));
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                player.setMove(sf::Vector2i(-1,0));
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                player.setMove(sf::Vector2i(0,-1));
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                player.setMove(sf::Vector2i(0,1));
            }
        }

        player.update();

        sf::Vector2i globalPosition = sf::Mouse::getPosition();

        // get the local mouse position (relative to a window)
        sf::Vector2i localPosition = sf::Mouse::getPosition(window);

        maze.setPlayerPosition(player.screenPos());
        maze.setCursorPosition(localPosition);

        sf::Vector2i playerPos = player.screenPos();
        sf::Vector2i desiredDir = localPosition - playerPos;

        player.setDirection(normalize(toF(desiredDir)));

        window.draw(player);
        window.draw(maze);

        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}
