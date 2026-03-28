#include "ghost.h"

Ghost::Ghost(float startX, float startY, int number, Display& win, Pacman& joueur)
    : m_x(startX), m_y(startY), m_number(number), m_display(win), m_pacman(joueur)
{}

Ghost::~Ghost() {}

void Ghost::update(float dt)
{
    int offset = m_display.getOffset();

    // Lorsqu'il est encore dans la base
    if (m_etat == 0)
    {
        // Déplacements verticaux
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
        
        // Temps de réaparition
        if (m_respawnTimer > 0.f)
        {
            m_respawnTimer -= dt;
            return;
        }

        // Sortie de la cage, phase 1 : centrage
        float exitX = 13 * 25.f;
        if (std::abs(m_x - exitX) > 1.f)
        {
            float moveX = (exitX > m_x ? 1.f : -1.f) * m_speed * dt;
            m_x += moveX;
            return;
        }
        else
        {
            m_x = exitX;
        }

        // Sortie de la cage, phase 2 : sortie
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

    // Lorsqu'il à le droit de se balader
    else if (m_etat == 1)
    {
        int currentTileX = (int)std::round(m_x / 25.f);
        int currentTileY = (int)std::round(m_y / 25.f);
        bool newTile = (currentTileX != m_lastTileX || currentTileY != m_lastTileY);

        float snappedX = currentTileX * 25.f;
        float snappedY = currentTileY * 25.f;
        bool isAligned = std::abs(snappedX - m_x) <= 4.f && std::abs(snappedY - m_y) <= 4.f;

        // --- Changement de direction à l'intersection ---
        if (isAligned && newTile)
        {
            m_lastTileX = currentTileX;
            m_lastTileY = currentTileY;

            std::vector<int> choices;
            for (int d = 0; d < 4; d++)
            {
                // Éviter le demi-tour (0<->2 et 1<->3)
                if (m_direction != -1 && (d == (m_direction + 2) % 4)) continue;

                int tx1, ty1, tx2, ty2;
                if (d == 3) { tx1 = tx2 = (int)((snappedX + m_size) / 25); ty1 = (int)((snappedY + 1 - offset) / 25); ty2 = (int)((snappedY + m_size - 1 - offset) / 25); }
                else if (d == 1) { tx1 = tx2 = (int)((snappedX - 1) / 25); ty1 = (int)((snappedY + 1 - offset) / 25); ty2 = (int)((snappedY + m_size - 1 - offset) / 25); }
                else if (d == 0) { ty1 = ty2 = (int)((snappedY - 1 - offset) / 25); tx1 = (int)((snappedX + 1) / 25); tx2 = (int)((snappedX + m_size - 1) / 25); }
                else { ty1 = ty2 = (int)((snappedY + m_size - offset) / 25); tx1 = (int)((snappedX + 1) / 25); tx2 = (int)((snappedX + m_size - 1) / 25); }

                int v1 = m_display.getMap(tx1, ty1), v2 = m_display.getMap(tx2, ty2);
                if (v1 != 1 && v1 != 4 && v1 != 0 && v2 != 1 && v2 != 4 && v2 != 0)
                    choices.push_back(d);
            }

            if (!choices.empty()) {
                m_x = snappedX;
                m_y = snappedY;
                m_direction = choices[std::rand() % choices.size()];
            }
        }

        // --- Déplacement ---
        int tx1, ty1, tx2, ty2;
        if (m_direction == 3) { tx1 = tx2 = (int)((m_x + m_size) / 25); ty1 = (int)((m_y + 1 - offset) / 25); ty2 = (int)((m_y + m_size - 1 - offset) / 25); }
        else if (m_direction == 1) { tx1 = tx2 = (int)((m_x - 1) / 25); ty1 = (int)((m_y + 1 - offset) / 25); ty2 = (int)((m_y + m_size - 1 - offset) / 25); }
        else if (m_direction == 0) { ty1 = ty2 = (int)((m_y - 1 - offset) / 25); tx1 = (int)((m_x + 1) / 25); tx2 = (int)((m_x + m_size - 1) / 25); }
        else if (m_direction == 2) { ty1 = ty2 = (int)((m_y + m_size - offset) / 25); tx1 = (int)((m_x + 1) / 25); tx2 = (int)((m_x + m_size - 1) / 25); }

        int v1 = m_display.getMap(tx1, ty1), v2 = m_display.getMap(tx2, ty2);
        if (m_direction != -1 && v1 != 1 && v1 != 4 && v1 != 0 && v2 != 1 && v2 != 4 && v2 != 0) {
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