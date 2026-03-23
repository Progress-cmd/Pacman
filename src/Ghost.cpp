#include "Ghost.h"

Ghost::Ghost(float startX, float startY, int number, Display& win, Pacman& joueur)
    : m_x(startX), m_y(startY), m_number(number), m_display(win), m_pacman(joueur)
{}

Ghost::~Ghost() {}

void Ghost::update(float dt)
{
    int offset = m_display.getOffset();

    if (m_etat == 0)
    {
        if (!m_released)
        {
            if (m_direction == 0)
            {
                int tileY  = (int)((m_y - 1 - offset) / 25);
                int tileX1 = (int)((m_x + 1) / 25);
                int tileX2 = (int)((m_x + m_size - 1) / 25);
                if (m_display.getMap(tileX1, tileY) == 1 || m_display.getMap(tileX2, tileY) == 1
                ||  m_display.getMap(tileX1, tileY) == 4 || m_display.getMap(tileX2, tileY) == 4)
                    m_direction = 2;
                else
                    m_y -= m_speed * dt;
            }
            else if (m_direction == 2)
            {
                int tileY  = (int)((m_y + m_size - offset) / 25);
                int tileX1 = (int)((m_x + 1) / 25);
                int tileX2 = (int)((m_x + m_size - 1) / 25);
                if (m_display.getMap(tileX1, tileY) == 1 || m_display.getMap(tileX2, tileY) == 1)
                    m_direction = 0;
                else
                    m_y += m_speed * dt;
            }
            else
            {
                m_direction = 0;
            }
            return;
        }
            
        if (m_respawnTimer > 0.f)
        {
            m_respawnTimer -= dt;
            return;
        }

        // Phase 1 : se recentrer sur la colonne de sortie (col 13 = x 325)
        float exitX = 13 * 25.f;
        if (std::abs(m_x - exitX) > 1.f)
        {
            float moveX = (exitX > m_x ? 1.f : -1.f) * m_speed * dt;
            m_x += moveX;
            return; // on ne monte pas encore
        }
        else
        {
            m_x = exitX; // snap précis
        }

        // Phase 2 : monter jusqu'à la sortie (ligne 14 = y 350 avec offset)
        m_y -= m_speed * dt;

        if (m_y <= 11 * 25.f + 150)
        {
            m_y = 11 * 25.f + 150;
            m_etat = 1;
            m_lastTileX = -1;
            m_lastTileY = -1;
            m_direction = 1; // part à gauche une fois sorti
        }
    }

    else if (m_etat == 1)
    {
        auto canMove = [&](int dir, float px, float py) -> bool {
            if (dir == 3) {
                int tileX  = (int)((px + m_size) / 25);
                int tileY1 = (int)((py + 1 - offset) / 25);
                int tileY2 = (int)((py + m_size - 1 - offset) / 25);
                return m_display.getMap(tileX, tileY1) != 1
                    && m_display.getMap(tileX, tileY2) != 1
                    && m_display.getMap(tileX, tileY1) != 4
                    && m_display.getMap(tileX, tileY2) != 4
                    && m_display.getMap(tileX, tileY1) != 0
                    && m_display.getMap(tileX, tileY2) != 0;
            }
            if (dir == 1) {
                int tileX  = (int)((px - 1) / 25);
                int tileY1 = (int)((py + 1 - offset) / 25);
                int tileY2 = (int)((py + m_size - 1 - offset) / 25);
                return m_display.getMap(tileX, tileY1) != 1
                    && m_display.getMap(tileX, tileY2) != 1
                    && m_display.getMap(tileX, tileY1) != 4
                    && m_display.getMap(tileX, tileY2) != 4
                    && m_display.getMap(tileX, tileY1) != 0
                    && m_display.getMap(tileX, tileY2) != 0;
            }
            if (dir == 0) {
                int tileY  = (int)((py - 1 - offset) / 25);
                int tileX1 = (int)((px + 1) / 25);
                int tileX2 = (int)((px + m_size - 1) / 25);
                return m_display.getMap(tileX1, tileY) != 1
                    && m_display.getMap(tileX2, tileY) != 1
                    && m_display.getMap(tileX1, tileY) != 4
                    && m_display.getMap(tileX2, tileY) != 4
                    && m_display.getMap(tileX1, tileY) != 0
                    && m_display.getMap(tileX2, tileY) != 0;
            }
            if (dir == 2) {
                int tileY  = (int)((py + m_size - offset) / 25);
                int tileX1 = (int)((px + 1) / 25);
                int tileX2 = (int)((px + m_size - 1) / 25);
                return m_display.getMap(tileX1, tileY) != 1
                    && m_display.getMap(tileX2, tileY) != 1
                    && m_display.getMap(tileX1, tileY) != 4
                    && m_display.getMap(tileX2, tileY) != 4
                    && m_display.getMap(tileX1, tileY) != 0
                    && m_display.getMap(tileX2, tileY) != 0;
            }
            return false;
        };

        auto opposite = [](int dir) -> int {
            if (dir == 0) return 2;
            if (dir == 2) return 0;
            if (dir == 1) return 3;
            if (dir == 3) return 1;
            return -1;
        };

        int currentTileX = (int)std::round(m_x / 25.f);
        int currentTileY = (int)std::round(m_y / 25.f);
        bool newTile = (currentTileX != m_lastTileX || currentTileY != m_lastTileY);

        float snappedX = currentTileX * 25.f;
        float snappedY = currentTileY * 25.f;
        const float snapTolerance = 4.f;
        bool isAligned = std::abs(snappedX - m_x) <= snapTolerance
                      && std::abs(snappedY - m_y) <= snapTolerance;

        if (isAligned && newTile)
        {
            m_lastTileX = currentTileX;
            m_lastTileY = currentTileY;

            std::vector<int> choices;
            for (int dir : {0, 1, 2, 3})
            {
                if (dir == opposite(m_direction)) continue;
                if (canMove(dir, snappedX, snappedY))
                    choices.push_back(dir);
            }
            if (!choices.empty())
            {
                m_x = snappedX;
                m_y = snappedY;
                m_direction = choices[std::rand() % choices.size()];
            }
        }

        if (m_direction != -1 && canMove(m_direction, m_x, m_y)) {
            if (m_direction == 3) m_x += m_speed * dt;
            if (m_direction == 1) m_x -= m_speed * dt;
            if (m_direction == 0) m_y -= m_speed * dt;
            if (m_direction == 2) m_y += m_speed * dt;
        }
    }

    // --- Collision avec Pacman ---
    float px = m_pacman.getX();
    float py = m_pacman.getY();

    float overlapX = std::abs(m_x - px);
    float overlapY = std::abs(m_y - py);

    m_invincibleTimer -= dt;
    if (overlapX < m_size && overlapY < m_size && m_invincibleTimer <= 0.f)
    {
        if (!m_pacman.getBoost())
        {
            m_pacman.reduceLife();
        }
        m_invincibleTimer = 2.f;

        // Retour en cage
        m_x = (12 + m_number) * 25.f;
        m_y = 20 * 25.f;
        m_etat = 0;
        m_released = true;
        m_respawnTimer = 3.f;
    }
}