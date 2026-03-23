#ifndef INPUT_H
#define INPUT_H

#include <SFML/Graphics.hpp>
#include <chrono>

#include "Display.h"


class Pacman
{
public:
    Pacman(float startX, float startY, Display& win);
    ~Pacman();

    void update(float dt);

    void reduceLife();

    float const getX();
    float const getY();
    int const getDirection();
    int const getScore();
    bool const getBoost();
    int const getLife();

    void setPosition(float x, float y);
    void upLife();

private:
    float m_x, m_y;

    float m_speed = 150.f;
    int m_direction = -1;

    int m_score = 0;
    int m_life = 3;

    bool m_boost = false;
    std::chrono::steady_clock::time_point m_startBoost;
    
    const float m_size = 25.f;
    Display& m_display;

    int m_wantedDirection = -1;
};

#endif