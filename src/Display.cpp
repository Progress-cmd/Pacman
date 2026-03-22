#include "Display.h"

Display::Display() : m_window(sf::VideoMode(m_size), "Pacman")
{
    if (!m_font.openFromFile("./assets/arial.ttf"))
    {
        std::cout << "Erreur de chargement de la police" << std::endl;
    }
}

Display::~Display() {}

void Display::createMap(int level, int highScore, int score, int nbLife)
{
    showInfo(level, highScore, score);
    for (int i = 0; i < y; i++)
    {
        for (int j = 0; j < x; j++)
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
        }
    }
    showLife(nbLife);
}

void Display::createSquare(float width, float height, sf::Color color, float posX, float posY)
{
    sf::RectangleShape rectangle(sf::Vector2f(width, height));
    rectangle.setFillColor(color);
    rectangle.setPosition(sf::Vector2f(posX, posY + m_offset));
    m_window.draw(rectangle);
}

void Display::createCircle(float ray, sf::Color color, float posX, float posY)
{
    sf::CircleShape cercle(ray);
    cercle.setFillColor(color);
    cercle.setPosition(sf::Vector2f(posX, posY + m_offset));
    m_window.draw(cercle);
}

void Display::createWall(int i, int j)
{
    sf::Color vide = colorMap[0];
    sf::Color mur = colorMap[1];
    createSquare(25, 25, vide, j * 25.f, i * 25.f);

    if (i < 1 || map[i-1][j] == 0 || map[i-1][j] == 2 || map[i-1][j] == 3)
    {
    createSquare(25, 3, mur, j * 25.f, i * 25.f);
    }
    if (i > 29 || map[i+1][j] == 0 || map[i+1][j] == 2 || map[i+1][j] == 3)
    {
    createSquare(25, 3, mur, j * 25.f, i * 25.f + 22.0f);
    }

    if (j < 1 || map[i][j-1] == 0 || map[i][j-1] == 2 || map[i][j-1] == 3)
    {
    createSquare(3, 25, mur, j * 25.f, i * 25.f);
    }
    if (j > 26 || map[i][j+1] == 0 || map[i][j+1] == 2 || map[i][j+1] == 3)
    {
    createSquare(3, 25, mur, j * 25.f + 22.0f, i * 25.f);
    }
}

sf::RenderWindow& Display::getWindow()
{
    return m_window;
}

void Display::updateMap(int x, int y)
{
    if ((y >= 0 && y <= 30) && (x >= 0 && x <= 27))
    {
        if (map[y][x] == 2 || map[y][x] == 3)
        {
            map[y][x] = 0;
        }
    }
}

int Display::getMap(int x, int y)
{
    if (x < 0 || x >= (int)this->x || y < 0 || y >= (int)this->y)
        return 1; // traiter les bords comme des murs
    return map[y][x];
}

void Display::showInfo(int level, int highScore, int score)
{
    sf::Text one(m_font);
    sf::Text one_1(m_font);
    sf::Text two(m_font);
    sf::Text two_1(m_font);
    sf::Text three(m_font);
    sf::Text three_1(m_font);

    one.setString("Level");
    one.setCharacterSize(35);
    one.setFillColor(colorMap[2]);
    one.setPosition(sf::Vector2f(100, 25));

    one_1.setString(std::to_string(level));
    one_1.setCharacterSize(35);
    one_1.setFillColor(colorMap[2]);
    one_1.setPosition(sf::Vector2f(100, 75));
    
    two.setString("High-scores");
    two.setCharacterSize(35);
    two.setFillColor(colorMap[2]);
    two.setPosition(sf::Vector2f(250, 25));

    two_1.setString(std::to_string(highScore));
    two_1.setCharacterSize(35);
    two_1.setFillColor(colorMap[2]);
    two_1.setPosition(sf::Vector2f(250, 75));
    
    three.setString("Scores");
    three.setCharacterSize(35);
    three.setFillColor(colorMap[2]);
    three.setPosition(sf::Vector2f(500, 25));

    three_1.setString(std::to_string(score));
    three_1.setCharacterSize(35);
    three_1.setFillColor(colorMap[2]);
    three_1.setPosition(sf::Vector2f(500, 75));
    
    m_window.draw(one);
    m_window.draw(one_1);
    m_window.draw(two);
    m_window.draw(two_1);
    m_window.draw(three);
    m_window.draw(three_1);
}

void Display::showLife(int nbLife)
{
    for (int i = 0; i < nbLife; i ++)
    {
        createCircle(12, colorMap[3], (24+9)*i + 9, y * pas + m_life - 33);

        sf::CircleShape triangle(12, 3);
        triangle.setFillColor(colorMap[0]);
        triangle.setOrigin({6.0f, 6.0f});
        triangle.setPosition(sf::Vector2f((24+9)*i + 9 + 6, y * pas + m_offset + m_life - 27));
        triangle.rotate(sf::degrees(90));
        m_window.draw(triangle);
    }
}

void Display::createPacman(float posex, float posey, int direction, int mouthAngle)
{
    const int points = 32; // résolution du cercle
    const float radius = 12.5f;

    // Angle de la bouche selon la direction
    float baseAngle = 0.f;
    if (direction == 0) baseAngle = 270.f;
    if (direction == 1) baseAngle = 180.f;
    if (direction == 2) baseAngle = 90.f;
    if (direction == 3) baseAngle = 0.f;


    sf::ConvexShape pacman;
    pacman.setPointCount(points + 1);

    // Centre du cercle
    pacman.setPoint(0, sf::Vector2f(radius, radius));

    for (int i = 0; i < points; i++)
    {
    float angle = (baseAngle + mouthAngle) + i * (360.f - 2.f * mouthAngle) / (float)(points - 1);
    float rad = angle * 3.14159265f / 180.f;
    float px = radius + radius * std::cos(rad);
    float py = radius + radius * std::sin(rad);
    pacman.setPoint(i + 1, sf::Vector2f(px, py));
    }

    pacman.setFillColor(sf::Color::Yellow);
    pacman.setPosition(sf::Vector2f(posex, posey));
    m_window.draw(pacman);
}

// Ajoute float time en paramètre
void Display::createGhosts(float posex, float posey, int number, float time)
{
    const int headPoints = 16;   // Points pour le demi-cercle du haut
    const int bottomPoints = 10; // Nombre de points pour les vagues du bas
    const float radius = 11.f;
    const float ghostHeight = 22.f;

    sf::ConvexShape ghost;
    ghost.setPointCount(headPoints + bottomPoints);

    // 1. Le haut du fantôme (demi-cercle)
    for (int i = 0; i < headPoints; i++) {
        float angle = i * 180.f / (headPoints - 1) + 180.f;
        float rad = angle * 3.14159f / 180.f;
        ghost.setPoint(i, sf::Vector2f(radius + radius * cos(rad), radius + radius * sin(rad)));
    }

    // 2. Le bas du fantôme (Vagues animées)
    for (int i = 0; i < bottomPoints; i++) {
        // On va de droite (radius*2) à gauche (0)
        float x = (radius * 2.f) - (i * (radius * 2.f) / (bottomPoints - 1));
        
        // Calcul de la vague : amplitude de 3 pixels
        // Le "time * 10" gère la vitesse de l'ondulation
        float wave = std::sin(time * 10.f + (x * 0.5f)) * 3.f;
        
        ghost.setPoint(headPoints + i, sf::Vector2f(x, ghostHeight + wave));
    }

    // --- Garde ton switch de couleur ici ---
    sf::Color colors[] = {sf::Color::Red, sf::Color(255,182,255), sf::Color::Cyan, sf::Color(255,182,85)};
    ghost.setFillColor(number < 4 ? colors[number] : sf::Color::White);
    
    ghost.setPosition(sf::Vector2f(posex, posey));
    m_window.draw(ghost);

    // --- Dessine les yeux ici (comme vu précédemment) ---
    ghost.setFillColor(number < 4 ? colors[number] : sf::Color::White);
    
    // Correction ici : on passe un Vector2f
    ghost.setPosition(sf::Vector2f(posex, posey));
    m_window.draw(ghost);

    // Yeux
    sf::CircleShape eye(3.f);
    eye.setFillColor(sf::Color::White);
    
    // Correction ici aussi
    eye.setPosition(sf::Vector2f(posex + radius * 0.4f, posey + radius * 0.6f));
    m_window.draw(eye);
    
    eye.setPosition(sf::Vector2f(posex + radius * 1.2f, posey + radius * 0.6f));
    m_window.draw(eye);

    // Pupilles
    sf::CircleShape pupil(1.5f);
    pupil.setFillColor(sf::Color::Blue);
    
    // Et encore ici
    pupil.setPosition(sf::Vector2f(posex + radius * 0.5f, posey + radius * 0.8f));
    m_window.draw(pupil);
    
    pupil.setPosition(sf::Vector2f(posex + radius * 1.3f, posey + radius * 0.8f));
    m_window.draw(pupil);
}