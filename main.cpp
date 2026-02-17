#include <SFML/Graphics.hpp>
#include <iostream>

#include "Display.h"


int main()
{
    // --- Initialisation de l'affichage --- //
    Display window;

    sf::RenderWindow& fenetre_obj = window.getWindow();

    // Boucle principale
    while (fenetre_obj.isOpen())
    {
        while (const std::optional event = fenetre_obj.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                fenetre_obj.close();
        }
        fenetre_obj.clear();
        window.createMap();
        fenetre_obj.display();
    }
    return 0;
}
