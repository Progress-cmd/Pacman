#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "input.h"
#include "display.h"
#include "ghost.h"


int main()
{
    // === Initialisation === //
    // --- Création des objets --- //
    Display window;
    Pacman pacman(14 * 25.f, 23 * 25.f, window);
    std::vector<Ghost*> fantomes;
    for (int i = 0; i < 4; i++)
    {
        Ghost* g = new Ghost((12+i) * 25.f, 20 * 25.f, i, window, pacman); 
        fantomes.push_back(g);
    }

    sf::RenderWindow& fenetre_obj = window.getWindow();
    fenetre_obj.setFramerateLimit(60);

    sf::Clock clock; // horloge spéfifique au déplacement
    sf::Clock animationClock; // horloge spécifique à l'animation de la bouche

    std::srand(std::time(nullptr));

    int lastLife = pacman.getLife();

    // --- Départ des fantômes --- //
    float releaseTimer = 0.f;
    int releasedCount = 0;
    const float releaseInterval = 5.f; // 1 fantôme toutes les 5 secondes

    bool win = false;

    // === Boucle principale === //
    while (fenetre_obj.isOpen())
    {
        // Boucle qui vérifie si la fenêtre est toujours ouverte
        while (const std::optional event = fenetre_obj.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                fenetre_obj.close();
        }

        // Vérification de victoire
        win = true;
        for (int i = 0; i < 31; i ++)
        {
            for (int j = 0; j < 28; j ++)
            {
                int value = window.getMap(i, j);
                if (value == 2 || value == 3)
                {
                    win = false;
                    break;
                }
            }
        }
        if (win) { std::cout << "Vous avez gagné !" << std::endl; break; }

        // Animation de la bouche du Pacman
        float time = animationClock.getElapsedTime().asSeconds();
        float speed = 5.0f;      // vitesse de battement
        float maxAngle = 35.f;   // ouverture max de la bouche

        float mouthAngle = std::abs(std::sin(time * speed)) * maxAngle;

        // Gestion des entitées
        float dt = clock.restart().asSeconds();
        pacman.update(dt);
        for (int i = 0; i < 4; i++)
        {
            fantomes[i]->update(dt);
        }

        // Vérification de défaite
        int currentLife = pacman.getLife();
        if (currentLife < lastLife)
        {
            pacman.setPosition(14 * 25.f, 23 * 25.f);
            lastLife = currentLife;
        }
        if (currentLife == 0) { std::cout << "Vous avez perdu !" << std::endl; break; }

        // Démarrage des fantômes, à la suite
        if (releasedCount < 4)
        {
            releaseTimer += dt;
            if (releaseTimer >= releaseInterval)
            {
                fantomes[releasedCount]->release();
                releasedCount++;
                releaseTimer = 0.f;
            }
        }

        // clear de la fenêtre
        fenetre_obj.clear();
        
        // rendu graphique
        window.createMap(0, 10, pacman.getScore(), pacman.getLife());
        window.createPacman(pacman.getX(), pacman.getY(), pacman.getDirection(), mouthAngle, pacman.getBoost());
        for (int i = 0; i < 4; i++)
        {
            window.createGhosts(fantomes[i]->getX(), fantomes[i]->getY(), fantomes[i]->getNumber(), time, pacman.getBoost());
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