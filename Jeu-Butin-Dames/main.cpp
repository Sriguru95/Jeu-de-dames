#include <iostream>
#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "ButinGame.hpp"
#include "DamesGame.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Jeux de plateau", sf::Style::Close);
    window.setFramerateLimit(60);

    Menu menu(window);

    while (window.isOpen()) {
        
        int selectedOption = menu.handleEvents();
        menu.display();

        if (selectedOption != -1) {
            if (selectedOption == 3) {
                window.close();
            } else {
                if (selectedOption == 0) {
                    
                    // Lance le jeu de Butin
                    ButinGame butinGame({"joueur1", "joueur2"});
                    butinGame.run(window);

                    // Vérifie si le retour au menu est demandé après avoir joué
                    if (butinGame.hasReturnToMenuRequested()) {
                        menu.resetSelectedOption();  // Réinitialise l'option sélectionnée dans le menu
                    }

                } else if (selectedOption == 1) {
                    // Lance le jeu de Butin
                    DamesGame DamesGame({"joueur1", "joueur2"});
                    DamesGame.run(window);

                    // Vérifie si le retour au menu est demandé après avoir joué
                    if (DamesGame.hasReturnToMenuRequested()) {
                        menu.resetSelectedOption();  // Réinitialise l'option sélectionnée dans le menu
                    }
                    
                } else if (selectedOption == 2) {
                    //Safari
                    std::cout << "Safari" << std::endl;
                }
            }
        } 

    }

    return 0;
}
