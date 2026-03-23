#ifndef DISPLAY_H
#define DISPLAY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <cmath>


class Display
{
public:
	Display();
	~Display();

	void createMap(int level, int highScore, int score, int nbLife);

    int updateMap(int x, int y);
    int getMap(int x, int y);

	sf::RenderWindow& getWindow();

    void createPacman(float posex, float posey, int direction, int mouthAngle, bool boost);
    void createGhosts(float posex, float posey, int number, float time, bool boost);

    int getSizeX() { return x; }
    int getSizeY() { return y; }
    int getPas() { return pas; }
    int getOffset() { return m_offset; }
    std::vector<unsigned int>& getBonus() { return m_bonus; }

private:
	static const unsigned int y = 31;
	static const unsigned int x = 28;

	unsigned int pas = 25;
	unsigned int m_offset = 150;
	unsigned int m_life = 42;

    std::vector<unsigned int> m_bonus = {1,1,2,3}; // bonus 1 : mangeur de fantome; bonus 2 : +500 points; bonus 3 : vie supplémentaire
	sf::Vector2u m_size = { x * pas, y * pas + m_offset + m_life};
	sf::RenderWindow m_window;
    sf::Font m_font;

    // 28 colonnes x 31 lignes
    int map[y][x] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, // 0
        {1,3,3,3,3,3,3,3,3,3,3,3,3,1,1,3,3,3,3,3,3,3,3,3,3,3,3,1}, // 1
        {1,3,1,1,1,1,3,1,1,1,1,1,3,1,1,3,1,1,1,1,1,3,1,1,1,1,3,1}, // 2
        {1,2,1,1,1,1,3,1,1,1,1,1,3,1,1,3,1,1,1,1,1,3,1,1,1,1,2,1}, // 3
        {1,3,1,1,1,1,3,1,1,1,1,1,3,1,1,3,1,1,1,1,1,3,1,1,1,1,3,1}, // 4
        {1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1}, // 5
        {1,3,1,1,1,1,3,1,1,3,1,1,1,1,1,1,1,1,3,1,1,3,1,1,1,1,3,1}, // 6
        {1,3,1,1,1,1,3,1,1,3,1,1,1,1,1,1,1,1,3,1,1,3,1,1,1,1,3,1}, // 7
        {1,3,3,3,3,3,3,1,1,3,3,3,3,1,1,3,3,3,3,1,1,3,3,3,3,3,3,1}, // 8
        {1,1,1,1,1,1,3,1,1,1,1,1,3,1,1,3,1,1,1,1,1,3,1,1,1,1,1,1}, // 9
        {0,0,0,0,0,1,3,1,1,1,1,1,3,1,1,3,1,1,1,1,1,3,1,0,0,0,0,0}, // 10
        {0,0,0,0,0,1,3,1,1,3,3,3,3,3,3,3,3,3,3,1,1,3,1,0,0,0,0,0}, // 11
        {0,0,0,0,0,1,3,1,1,3,1,1,1,4,4,1,1,1,3,1,1,3,1,0,0,0,0,0}, // 12
        {1,1,1,1,1,1,3,1,1,3,1,0,0,0,0,0,0,1,3,1,1,3,1,1,1,1,1,1}, // 13
        {0,0,0,0,0,0,3,3,3,3,1,0,0,0,0,0,0,1,3,3,3,3,0,0,0,0,0,0}, // 14
        {1,1,1,1,1,1,3,1,1,3,1,0,0,0,0,0,0,1,3,1,1,3,1,1,1,1,1,1}, // 15
        {0,0,0,0,0,1,3,1,1,3,1,1,1,1,1,1,1,1,3,1,1,3,1,0,0,0,0,0}, // 16
        {0,0,0,0,0,1,3,1,1,3,3,3,3,3,3,3,3,3,3,1,1,3,1,0,0,0,0,0}, // 17
        {0,0,0,0,0,1,3,1,1,3,1,1,1,1,1,1,1,1,3,1,1,3,1,0,0,0,0,0}, // 18
        {1,1,1,1,1,1,3,1,1,3,1,1,1,1,1,1,1,1,3,1,1,3,1,1,1,1,1,1}, // 19
        {1,3,3,3,3,3,3,3,3,3,3,3,3,1,1,3,3,3,3,3,3,3,3,3,3,3,3,1}, // 20
        {1,3,1,1,1,1,3,1,1,1,1,1,3,1,1,3,1,1,1,1,1,3,1,1,1,1,3,1}, // 21
        {1,3,1,1,1,1,3,1,1,1,1,1,3,1,1,3,1,1,1,1,1,3,1,1,1,1,3,1}, // 22
        {1,2,3,3,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,3,3,2,1}, // 23
        {1,1,1,3,1,1,3,1,1,3,1,1,1,1,1,1,1,1,3,1,1,3,1,1,3,1,1,1}, // 24
        {1,1,1,3,1,1,3,1,1,3,1,1,1,1,1,1,1,1,3,1,1,3,1,1,3,1,1,1}, // 25
        {1,3,3,3,3,3,3,1,1,3,3,3,3,1,1,3,3,3,3,1,1,3,3,3,3,3,3,1}, // 26
        {1,3,1,1,1,1,1,1,1,1,1,1,3,1,1,3,1,1,1,1,1,1,1,1,1,1,3,1}, // 27
        {1,3,1,1,1,1,1,1,1,1,1,1,3,1,1,3,1,1,1,1,1,1,1,1,1,1,3,1}, // 28
        {1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1}, // 29
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}  // 30
    };


	std::unordered_map<int, sf::Color> colorMap = {
		{0, sf::Color::Black},  // Vide
		{1, sf::Color::Blue},   // Mur
        {2, sf::Color::White},  // Super Pac gom
        {3, sf::Color::Yellow},  // Pac gom
        {4, sf::Color(255, 182, 193)}, // rose clair mur spawn
        {5, sf::Color::Black}
	};

	void createSquare(float width, float height, sf::Color color, float posX, float posY);
    void createCircle(float ray, sf::Color color, float posX, float posY);
    void createWall(int i, int j);

    void showInfo(int level, int highScore, int score);
    void showLife(int nbLife);
};

#endif