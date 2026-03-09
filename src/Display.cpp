#include "Display.h"

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

            if (id == 0)
            {
                sf::Color c = colorMap[0];
                createSquare(25, 25, c, j * 25.f, i * 25.f);
            }
            else if (id == 1)
            {
                createWall(i, j);
            }
            else if (id == 2)
            {
                sf::Color c = colorMap[0];
                createSquare(25, 25, c, j * 25.f, i * 25.f);
                sf::Color c1 = colorMap[2];
                createCircle(7, c1, j * 25.f + 5.5f, i * 25.f + 5.5f);
            }
            else if (id == 3)
            {
                sf::Color c = colorMap[0];
                createSquare(25, 25, c, j * 25.f, i * 25.f);
                sf::Color c1 = colorMap[3];
                createCircle(5, c1, j * 25.f + 7.5f, i * 25.f + 7.5f);
            }
            else
            {
                sf::Color c = colorMap[id];
                createSquare(25, 25, c, j * 25.f, i * 25.f);
            }
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

void Display::createCircle(float ray, sf::Color color, float posX, float posY)
{
    sf::CircleShape cercle(ray);
    cercle.setFillColor(color);
    cercle.setPosition(sf::Vector2f(posX, posY));
    m_window.draw(cercle);
}

void Display::createWall(int i, int j)
{
    sf::Color vide = colorMap[0];
    sf::Color mur = colorMap[1];
    createSquare(25, 25, vide, j * 25.f, i * 25.f);

    if (i < 1 || map[i-1][j] == 3)
    {
    createSquare(5, 25, mur, j * 25.f, i * 20.f);
    }
}

sf::RenderWindow& Display::getWindow()
{
    return m_window;
}