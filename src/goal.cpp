#include <goal.hpp>

#include <SFML/OpenGL.hpp>

Goal::Goal(const int ROW, const int COLUMN) : _pos(sf::Vector2i(ROW, COLUMN))
{
}

void Goal::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Vector2u viewport = target.getSize();
    glViewport(0, 0, viewport.x, viewport.y);

    glMatrixMode(GL_MODELVIEW);
    glTranslatef(_pos.y + 0.5f, _pos.x + 0.5f, 0);

    const float RADIUS = 0.1f;
    glColor3f(0,1,1);
    glBegin(GL_QUADS);
    {
        glVertex2f(-RADIUS, -RADIUS);
        glVertex2f(RADIUS, -RADIUS);
        glVertex2f(RADIUS, RADIUS);
        glVertex2f(-RADIUS, RADIUS);
    }
    glEnd();
}
