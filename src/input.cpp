#include "input.h"

Pacman::Pacman(float startX, float startY, Display& win) : m_x(startX), m_y(startY), m_display(win) {}

Pacman::~Pacman() {}

void Pacman::update(float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
        m_direction = 3;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
        m_direction = 1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
        m_direction = 0;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
        m_direction = 2;
    }

    if (m_direction == 3){
        m_x += m_speed * dt;
    }
    else if (m_direction == 1){
        m_x += -m_speed * dt;
    }
    else if (m_direction == 0){
        m_y += -m_speed * dt;
    }
    else if (m_direction == 2){
        m_y += m_speed * dt;
    }

    if (m_x < 0)
    {
        m_x = m_display.getSizeX()*m_display.getPas();
    }
    if (m_x > m_display.getSizeX()*m_display.getPas())
    {
        m_x = 0;
    }
}

float const Pacman::getX()
{
    return m_x;
}

float const Pacman::getY()
{
    return m_y;
}

int const Pacman::getDirection()
{
    return m_direction;
}