#include "Ghost.h"

Ghost::Ghost(float startX, float startY, int number, Display& win)
    : m_x(startX), m_y(startY), m_number(number), m_display(win)
{}

Ghost::~Ghost() {}

void Ghost::update(float dt)
{
    int offset = m_display.getOffset();

    if (m_etat == 0)
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
    }

    else if (m_etat == 1)
    {
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

        auto opposite = [](int dir) -> int {
            if (dir == 0) return 2;
            if (dir == 2) return 0;
            if (dir == 1) return 3;
            if (dir == 3) return 1;
            return -1;
        };

        // Tuile courante (arrondie)
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

        // Déplacement
        if (m_direction != -1 && canMove(m_direction, m_x, m_y)) {
            if (m_direction == 3) m_x += m_speed * dt;
            if (m_direction == 1) m_x -= m_speed * dt;
            if (m_direction == 0) m_y -= m_speed * dt;
            if (m_direction == 2) m_y += m_speed * dt;
        }
    }
}