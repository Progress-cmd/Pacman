#ifndef GHOST_H
#define GHOST_H

#include "Display.h"

class Ghost
{
public:
    Ghost(float startX, float startY, int nubmer, Display& win);
    ~Ghost();

    void update(float dt);

    int getNumber() { return m_number; }
    const float getX() { return m_x; }
    const float getY() { return m_y; }

protected:
    int const m_number;
    float m_x, m_y;
    
    float const m_speed = 75.f;
    int m_direction = 1;

    const float m_size = 25.f;
    Display& m_display;

    int m_etat = 1;
    // 0 : repos, dans la cage
    // 1 : sorti, il se déplace dans la map
    // 2 : attaque (fonctionnalité supplémentaire)
    // 3 : mort, mangé il retourne à la base

    bool m_hasChosenOnThisTile = false;
    int m_lastTileX = -1;
    int m_lastTileY = -1;
};

#endif