#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "input.h"
#include "Display.h"


int main()
{
    // --- Initialisation --- //
    Display window;

    sf::RenderWindow& fenetre_obj = window.getWindow();

    Pacman pacman(14 * 25.f, 23 * 25.f, window);

    sf::Clock clock; // horloge spéfifique au déplacement
    sf::Clock animationClock; // horloge spécifique à l'animation de la bouche

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
        pacman.update(clock.restart().asSeconds());
        
        // clear de la fenêtre
        fenetre_obj.clear();
        
        // rendu graphique
        window.createMap(0, 10, 4, 3);
        window.createPacman(pacman.getX(), pacman.getY(), pacman.getDirection(), mouthAngle);

        // affichage de la fenêtre
        fenetre_obj.display();
    }
    return 0;
}