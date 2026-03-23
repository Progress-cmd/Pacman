#include "input.h"

Pacman::Pacman(float startX, float startY, Display& win) : m_x(startX), m_y(startY), m_display(win) {}

Pacman::~Pacman() {}

void Pacman::update(float dt)
{
    int offset = m_display.getOffset();

    // --- Capture de la direction souhaitée ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        m_wantedDirection = 3;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        m_wantedDirection = 1;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        m_wantedDirection = 0;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        m_wantedDirection = 2;

    // --- Collision ---
    auto canMove = [&](int dir, float px, float py) -> bool {
        if (dir == 3) {
            int tileX  = (int)((px + m_size) / 25);
            int tileY1 = (int)((py + 1 - offset) / 25);
            int tileY2 = (int)((py + m_size - 1 - offset) / 25);
            return m_display.getMap(tileX, tileY1) != 1
                && m_display.getMap(tileX, tileY2) != 1;
        }
        if (dir == 1) {
            int tileX  = (int)((px - 1) / 25);
            int tileY1 = (int)((py + 1 - offset) / 25);
            int tileY2 = (int)((py + m_size - 1 - offset) / 25);
            return m_display.getMap(tileX, tileY1) != 1
                && m_display.getMap(tileX, tileY2) != 1;
        }
        if (dir == 0) {
            int tileY  = (int)((py - 1 - offset) / 25);
            int tileX1 = (int)((px + 1) / 25);
            int tileX2 = (int)((px + m_size - 1) / 25);
            return m_display.getMap(tileX1, tileY) != 1
                && m_display.getMap(tileX2, tileY) != 1;
        }
        if (dir == 2) {
            int tileY  = (int)((py + m_size - offset) / 25);
            int tileX1 = (int)((px + 1) / 25);
            int tileX2 = (int)((px + m_size - 1) / 25);
            return m_display.getMap(tileX1, tileY) != 1
                && m_display.getMap(tileX2, tileY) != 1;
        }
        return false;
    };

    // --- 3. Snap + changement de direction ---
    // On ne snap que si le changement est perpendiculaire
    bool horizontal = (m_direction == 1 || m_direction == 3);
    bool wantVertical   = (m_wantedDirection == 0 || m_wantedDirection == 2);
    bool wantHorizontal = (m_wantedDirection == 1 || m_wantedDirection == 3);

    if (m_wantedDirection != -1)
    {
        float snappedX = m_x;
        float snappedY = m_y;

        // Si on veut aller en vertical alors qu'on va en horizontal : snap sur X
        if (horizontal && wantVertical)
            snappedX = std::round(m_x / 25.f) * 25.f;

        // Si on veut aller en horizontal alors qu'on va en vertical : snap sur Y
        if (!horizontal && wantHorizontal)
            snappedY = std::round(m_y / 25.f) * 25.f;

        // On autorise le snap seulement si on est proche (<= snapTolerance px)
        const float snapTolerance = 6.f;
        bool snapOk = std::abs(snappedX - m_x) <= snapTolerance
                   && std::abs(snappedY - m_y) <= snapTolerance;

        if (snapOk && canMove(m_wantedDirection, snappedX, snappedY))
        {
            m_x = snappedX;
            m_y = snappedY;
            m_direction = m_wantedDirection;
        }
    }

    // --- Déplacement ---
    if (m_direction != -1 && canMove(m_direction, m_x, m_y)) {
        if (m_direction == 3) m_x += m_speed * dt;
        if (m_direction == 1) m_x -= m_speed * dt;
        if (m_direction == 0) m_y -= m_speed * dt;
        if (m_direction == 2) m_y += m_speed * dt;
    }

    // --- Téléportation ---
    if (m_x < 0)
        m_x = (float)(m_display.getSizeX() - 1) * m_display.getPas();
    if (m_x > (float)(m_display.getSizeX() - 1) * m_display.getPas())
        m_x = 0;
    m_wantedDirection = -1;

    // --- Scores ---
    if (m_display.updateMap((m_x+12.5)/25, ((m_y+12.5) - m_display.getOffset())/25) == 1)
    {
        m_score += 100;
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

int const Pacman::getScore()
{
    return m_score;
}