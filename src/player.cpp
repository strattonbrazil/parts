#include <player.hpp>
#include <constants.hpp>

#include <SFML/OpenGL.hpp>

Player::Player() : _moving(false), _from(0,0)
{

}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Vector2u viewport = target.getSize();
    glViewport(0, 0, viewport.x, viewport.y);

    const int HALF_CELL = CELL_OFFSET / 2;

    sf::Vector2f playerPos = mazePos();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, viewport.x, viewport.y, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(MAZE_OFFSET + HALF_CELL + playerPos.y * CELL_OFFSET,
                 MAZE_OFFSET + HALF_CELL + playerPos.x * CELL_OFFSET, 0);

    const int ROW = _from.x;
    const int HEIGHT = _from.y;

    const float RADIUS = 2.5f;
    glColor3f(0,1,0);
    glBegin(GL_QUADS);
    {
        glVertex2f(-RADIUS, -RADIUS);
        glVertex2f(RADIUS, -RADIUS);
        glVertex2f(RADIUS, RADIUS);
        glVertex2f(-RADIUS, RADIUS);
    }
    glEnd();

    const int DIR_LENGTH = CELL_OFFSET * 0.4f;
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    {
        glVertex2f(0,0);
        glVertex2f(DIR_LENGTH * _direction.x, DIR_LENGTH * _direction.y);
    }
    glEnd();
}

void Player::update()
{
    if (_moving) {
        _transition += 0.002f;
        if (_transition > 0.99999f) {
            _moving = false;
            _from = _to;
            _transition = 0;
        }
    }
}

void Player::setMove(sf::Vector2i dir)
{
    _to = _from + dir;
    _transition = 0.0f;
    _moving = true;
}

sf::Vector2f Player::mazePos() const
{
    return sf::Vector2f(_from.x * (1 - _transition) + _to.x * _transition,
                        _from.y * (1 - _transition) + _to.y * _transition);
}

sf::Vector2i Player::screenPos() const
{
    const int HALF_CELL = CELL_OFFSET / 2;

    sf::Vector2f current = mazePos();
    sf::Vector2i swizzled(CELL_OFFSET * current.y, CELL_OFFSET * current.x);
    return sf::Vector2i(MAZE_OFFSET + HALF_CELL, MAZE_OFFSET + HALF_CELL) + swizzled;
}
