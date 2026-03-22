#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "input.h"
#include "Display.h"
#include "Ghost.h"


int main()
{
    // --- Initialisation --- //
    Display window;

    sf::RenderWindow& fenetre_obj = window.getWindow();
    fenetre_obj.setFramerateLimit(60);

    Pacman pacman(14 * 25.f, 23 * 25.f, window);

    sf::Clock clock; // horloge spéfifique au déplacement
    sf::Clock animationClock; // horloge spécifique à l'animation de la bouche

    std::vector<Ghost*> fantomes;
    for (int i = 0; i < 4; i++)
    {
        Ghost* g = new Ghost((12+i) * 25.f, 20 * 25.f, i, window); 
        fantomes.push_back(g);
    }

    std::srand(std::time(nullptr));

    // --- Boucle principale --- //
    while (fenetre_obj.isOpen())
    {
        // boucle qui vérifie si la fenêtre est toujours ouverte
        while (const std::optional event = fenetre_obj.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                fenetre_obj.close();
        }


        // animation de la bouche
        float time = animationClock.getElapsedTime().asSeconds();
        float speed = 5.0f;      // vitesse de battement
        float maxAngle = 35.f;   // ouverture max de la bouche

        float mouthAngle = std::abs(std::sin(time * speed)) * maxAngle;

        // gestion du pacman
        float dt = clock.restart().asSeconds();
        pacman.update(dt);
        for (int i = 0; i < 4; i++)
        {
            fantomes[i]->update(dt);
        }

        
        // clear de la fenêtre
        fenetre_obj.clear();
        
        // rendu graphique
        window.createMap(0, 10, 4, 3);
        window.createPacman(pacman.getX(), pacman.getY(), pacman.getDirection(), mouthAngle);
        for (int i = 0; i < 4; i++)
        {
            window.createGhosts(fantomes[i]->getX(), fantomes[i]->getY(), fantomes[i]->getNumber(), time);
        }

        // affichage de la fenêtre
        fenetre_obj.display();
    }
    
    for (Ghost* g : fantomes)
    {
        delete g;
    }
    fantomes.clear();

    return 0;
}