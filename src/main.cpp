#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <maze.hpp>

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

        sf::Vector2i globalPosition = sf::Mouse::getPosition();

        // get the local mouse position (relative to a window)
        sf::Vector2i localPosition = sf::Mouse::getPosition(window);

        maze.setCursorPosition(localPosition);

        window.draw(maze);

        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}
