#include "display.h"

Display::Display() :m_window(sf::VideoMode(m_size), "Pacman") { }

Display::~Display()
{

}

void Display::createMap()
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            int id = map[i][j];
            sf::Color c = colorMap[id];
            createSquare(25, 25, c, j * 25.f, i * 25.f);
        }
    }
}

void Display::createSquare(float width, float height, sf::Color color, float posX, float posY)
{
    sf::RectangleShape rectangle(sf::Vector2f(width, height));
    rectangle.setFillColor(color);
    rectangle.setPosition(sf::Vector2f(posX, posY));
    m_window.draw(rectangle);
}

sf::RenderWindow& Display::getWindow()
{
    return m_window;
}