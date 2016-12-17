#include <player.hpp>

#include <SFML/OpenGL.hpp>

Player::Player() : _moving(false), _from(0,0)
{

}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Vector2u viewport = target.getSize();
    glViewport(0, 0, viewport.x, viewport.y);

    sf::Vector2f playerPos = mazePos();

    glMatrixMode(GL_MODELVIEW);
    glTranslatef(playerPos.y, playerPos.x, 0);

    const float RADIUS = 0.1f;
    glColor3f(0,1,0);
    glBegin(GL_QUADS);
    {
        glVertex2f(-RADIUS, -RADIUS);
        glVertex2f(RADIUS, -RADIUS);
        glVertex2f(RADIUS, RADIUS);
        glVertex2f(-RADIUS, RADIUS);
    }
    glEnd();
}

void Player::update()
{
    if (_moving) {
        _transition += 0.001f;
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
    // add 0.5 so player centered in cell
    return sf::Vector2f(_from.x * (1 - _transition) + _to.x * _transition,
                        _from.y * (1 - _transition) + _to.y * _transition) + sf::Vector2f(0.5f, 0.5f);
}
