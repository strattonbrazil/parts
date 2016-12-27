#include <minigame.hpp>

#include <SFML/OpenGL.hpp>
#include <boost/python.hpp>

#include <iostream>

MiniGame::MiniGame()
{
    _gameContext = dict();
    _gameContext["finished"] = false;
    _finished = false;

    object main_module = import("__main__");
    object main_namespace = main_module.attr("__dict__");

    object minigameLib = import("minigame");
    _minigameUpdateFunc = minigameLib.attr("update_game");
}

void parseArray(object pyArray, float* array, int count)
{
    for (int i = 0; i < count; i++) {
        array[i] = extract<float>(object(pyArray[i]));
    }
}

void drawRectangle(float* position, float* size, float* color)
{
    glColor4f(color[0], color[1], color[2], 1);
    glBegin(GL_QUADS);
    {
        glVertex2f(position[0], position[1]);
        glVertex2f(position[0]+size[0], position[1]);
        glVertex2f(position[0]+size[0], position[1]+size[1]);
        glVertex2f(position[0], position[1]+size[1]);
    }
    glEnd();
}

void MiniGame::update(const float elapsed, sf::Vector2f mousePos)
{
    _gameContext["elapsed"] = elapsed
    _gameContext["mouseDown"] = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    _gameContext["mousePos"] = make_tuple(mousePos.x, mousePos.y);
    
    _minigameUpdateFunc(_gameContext);
    object finished = _gameContext["finished"];
    if (!finished.is_none()) {
        _finished = extract<bool>(_gameContext["finished"]);
    }
}

void MiniGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Vector2u viewport = target.getSize();
    glViewport(0, 0, viewport.x, viewport.y);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glClearColor(0,0,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    object assets = _gameContext["assets"];
    for (int i = 0; i < len(assets); i++) {
        float position[2];
        float color[3];
        float size[2];
        object asset = assets[i];
        char const* type = extract<char const*>(object(asset["type"]));
        if (strcmp(type, "rectangle") == 0) {
            parseArray(object(asset["color"]), color, 3);
            parseArray(object(asset["position"]), position, 2);
            parseArray(object(asset["size"]), size, 2);
            drawRectangle(position, size, color);
        }
    }

    // TODO: draw border


    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
