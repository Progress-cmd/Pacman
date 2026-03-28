#include "input.h"

Pacman::Pacman(float startX, float startY, Display& win) : m_x(startX), m_y(startY), m_display(win) {}

Pacman::~Pacman() {}

void Pacman::update(float dt)
{
    int offset = m_display.getOffset();

    // --- Direction souhaitée ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) m_wantedDirection = 3;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  m_wantedDirection = 1;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    m_wantedDirection = 0;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  m_wantedDirection = 2;

    // --- Snap + changement de direction ---
    // Le snap permet de recentrer le pacman sur les couloirs
    if (m_wantedDirection != -1)
    {
        float snappedX = m_x;
        float snappedY = m_y;

        // On calcule le snap selon l'axe de mouvement actuel
        if (m_direction == 1 || m_direction == 3) { snappedX = std::round(m_x / 25.f) * 25.f; } // Horizontal
        else { snappedY = std::round(m_y / 25.f) * 25.f; } // Vertical

        if (std::abs(snappedX - m_x) <= 6.f && std::abs(snappedY - m_y) <= 6.f)
        {
            // Test de collision pour m_wantedDirection
            int tx1, ty1, tx2, ty2;
            if (m_wantedDirection == 3) { tx1 = tx2 = (int)((snappedX + m_size) / 25); ty1 = (int)((snappedY + 1 - offset) / 25); ty2 = (int)((snappedY + m_size - 1 - offset) / 25); }
            else if (m_wantedDirection == 1) { tx1 = tx2 = (int)((snappedX - 1) / 25); ty1 = (int)((snappedY + 1 - offset) / 25); ty2 = (int)((snappedY + m_size - 1 - offset) / 25); }
            else if (m_wantedDirection == 0) { ty1 = ty2 = (int)((snappedY - 1 - offset) / 25); tx1 = (int)((snappedX + 1) / 25); tx2 = (int)((snappedX + m_size - 1) / 25); }
            else { ty1 = ty2 = (int)((snappedY + m_size - offset) / 25); tx1 = (int)((snappedX + 1) / 25); tx2 = (int)((snappedX + m_size - 1) / 25); }

            int v1 = m_display.getMap(tx1, ty1), v2 = m_display.getMap(tx2, ty2);
            if (v1 != 1 && v1 != 4 && v2 != 1 && v2 != 4) {
                m_x = snappedX;
                m_y = snappedY;
                m_direction = m_wantedDirection;
            }
        }
    }

    // --- Déplacement ---
    if (m_direction != -1)
    {
        int tx1, ty1, tx2, ty2;
        if (m_direction == 3) { tx1 = tx2 = (int)((m_x + m_size) / 25); ty1 = (int)((m_y + 1 - offset) / 25); ty2 = (int)((m_y + m_size - 1 - offset) / 25); }
        else if (m_direction == 1) { tx1 = tx2 = (int)((m_x - 1) / 25); ty1 = (int)((m_y + 1 - offset) / 25); ty2 = (int)((m_y + m_size - 1 - offset) / 25); }
        else if (m_direction == 0) { ty1 = ty2 = (int)((m_y - 1 - offset) / 25); tx1 = (int)((m_x + 1) / 25); tx2 = (int)((m_x + m_size - 1) / 25); }
        else { ty1 = ty2 = (int)((m_y + m_size - offset) / 25); tx1 = (int)((m_x + 1) / 25); tx2 = (int)((m_x + m_size - 1) / 25); }

        int v1 = m_display.getMap(tx1, ty1), v2 = m_display.getMap(tx2, ty2);
        if (v1 != 1 && v1 != 4 && v2 != 1 && v2 != 4) {
            if (m_direction == 3) m_x += m_speed * dt;
            if (m_direction == 1) m_x -= m_speed * dt;
            if (m_direction == 0) m_y -= m_speed * dt;
            if (m_direction == 2) m_y += m_speed * dt;
        }
    }

    // --- Téléportation ---
    float maxX = (float)(m_display.getSizeX() - 1) * 25.f;
    if (m_x < 0) m_x = maxX;
    else if (m_x > maxX) m_x = 0;
    
    m_wantedDirection = -1;

    // --- Scores et Bonus ---
    int type = m_display.updateMap((m_x + 12.5f) / 25, ((m_y + 12.5f) - offset) / 25);
    if (type == 1) m_score += 100;
    else if (type == 2)
    {
        std::vector<unsigned int>& bonus = m_display.getBonus();
        if (!bonus.empty()) {
            int index = std::rand() % bonus.size();
            int valeur = bonus[index];
            bonus.erase(bonus.begin() + index);

            if (valeur == 1) { m_startBoost = std::chrono::steady_clock::now(); m_boost = true; }
            else if (valeur == 2) m_score += 500;
            else if (valeur == 3) upLife();
        }
    }

    if (m_boost)
    {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - m_startBoost).count() > 10)
            m_boost = false;
    }
}

void Pacman::reduceLife()
{
    m_life -= 1;
}

int const Pacman::getLife()
{
    return m_life;
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

int const Pacman::getScore()
{
    return m_score;
}

bool const Pacman::getBoost()
{
    return m_boost;
}

void Pacman::upLife()
{
    m_life += 1;
}

void Pacman::setPosition(float x, float y)
{ 
    m_x = x; 
    m_y = y; 
    m_direction = -1; 
    m_wantedDirection = -1;
}