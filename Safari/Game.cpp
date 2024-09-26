#include "Game.hpp"
#include "Player.hpp"
#include <iostream>
#include <vector>

// Constructeur
Game::Game() {
    // Initialiser les joueurs et permettre le choix des animaux
    std::vector<char> availableAnimals = {'R', 'L', 'E'};
    // Game::Game()...
    for (int i = 0; i < numPlayers; ++i) {
        char animalChoice;
        auto it = availableAnimals.end();

        do {
            // Afficher les animaux disponibles
            std::cout << "Available animals: ";
            for (char availableAnimal : availableAnimals) {
                std::cout << availableAnimal << " ";
            }
            std::cout << std::endl;

            // Demander à l'utilisateur de choisir un animal
            std::cout << "Player " << i + 1 << ", choose your animal: ";
            std::cin >> animalChoice;

            // Vérifier si l'animal choisi est disponible
            it = std::find(availableAnimals.begin(), availableAnimals.end(), animalChoice);

            if (it != availableAnimals.end()) {
                // Animal choisi est disponible, retirer de la liste des animaux disponibles
                availableAnimals.erase(it);

                // Créer l'instance de Player avec le choix valide
                players[i] = Player(i + 1, animalChoice);
            } else {
                // Animal choisi n'est pas disponible
                std::cout << "Invalid choice. Please choose 'R', 'L', or 'E'." << std::endl;
            }
        } while (it == availableAnimals.end() && !availableAnimals.empty());  // Continuer tant que la saisie n'est pas valide et la liste n'est pas vide
    }


}


// Méthode pour démarrer la partie
void Game::start() {
    int currentPlayerIndex = 0;
    mettrePiece();

    /*
    //while (!isGameOver()) {
        // Afficher le plateau avant chaque tour
        std::cout << "Current Board:" << std::endl << board << std::endl;

        // Tour du joueur actuel
        playTurn(players[currentPlayerIndex]);

        // Passer au joueur suivant
        currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
    //}
    std::cout << "FIN" << std::endl;

    // Afficher le gagnant
    */
}



// Méthode pour gérer les tours des joueurs
void Game::mettrePiece() {
    //for (int i = 0; i < 3; ++i) {
        for (Player& currentPlayer : players) {
            std::cout << "Player " << currentPlayer.getID() << "'s turn." << std::endl;

            // Afficher les pièces du joueur
            currentPlayer.displayPiecesWithCoordinates();

            // Placer trois pièces sur le plateau
                // Demander au joueur de choisir une pièce
                int pieceIndex;
                char pieceLetter;
                
                while (true) {
                    // Demander au joueur de choisir une pièce
                    std::cout << "Choose a piece (enter the letter and index): ";
                    std::cin >> pieceLetter >> pieceIndex;

                    if (std::cin.fail()) {
                        std::cin.clear();  // Réinitialiser l'état de cin après une erreur
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignorer le reste de la ligne
                        std::cout << "Invalid input. Saisie fausse recommencer" << std::endl;
                        continue;
                    } 

                    // Vérifier si la lettre correspond au type d'animal du joueur
                    if (pieceLetter != currentPlayer.getAnimalType()) {
                        std::cout << "Invalid choice. Piece type does not match your animal type." << std::endl;
                        continue;  // Revenir au début de la boucle
                    }

                    // Vérifier si l'index est compris entre 1 et 3
                    if (pieceIndex < 0 || pieceIndex > 2) {
                        std::cout << "Invalid choice. Please enter an index between 0 and 2." << std::endl;
                        continue;  // Revenir au début de la boucle
                    }

                    // Récupérer la pièce correspondant à l'index
                    /*
                    if (pieceIndex >= 0 && pieceIndex <= 2){
                        Piece* selectedPiece = currentPlayer.getPiece(pieceIndex);

                        // Vérifier si les coordonnées de la pièce sont différentes de -1 (déjà placée)
                        if (selectedPiece->getX() != -1 || selectedPiece->getY() != -1) {
                            std::cout << "Invalid choice. Piece is already placed. Choose another piece." << std::endl;
                            continue;  // Revenir au début de la boucle
                        }
                    }
                    */

                    break;

                    // Si toutes les conditions sont remplies, la boucle s'arrête
                } // La condition ici peut être ajustée selon vos besoins


                // Demander au joueur de choisir des coordonnées
                int newX, newY;
                do {
                    std::cout << "Enter the coordinates to place the piece (x y): ";
                    std::cin >> newX >> newY;

                    if (newX < 0 || newX >= Board::boardSize || newY < 0 || newY >= Board::boardSize) {
                        // Afficher un message d'erreur si les coordonnées ne sont pas valides
                        std::cout << "Invalid coordinates. Please enter values between 0 and " << Board::boardSize - 1 << "." << std::endl;
                    }

                } while ((newX < 0 || newX >= Board::boardSize || newY < 0 || newY >= Board::boardSize || board.isPositionOccupied(newX, newY)));

                // Déplacer la pièce sur le plateau
                currentPlayer.placePiece(pieceIndex, newX, newY);

                // Mettre à jour le plateau
                board.placePiece(newX, newY,*currentPlayer.getPiece(pieceIndex));

                // Afficher le plateau après chaque coup
                std::cout << "Current Board:" << std::endl << board << std::endl;

            std::cout << "Player " << currentPlayer.getID() << "'s turn is over." << std::endl;
        }
    //}
}



// Méthode pour vérifier la fin de la partie
bool Game::isGameOver() const {
    // Implémenter la logique de fin de partie ici
    // Par exemple, vérifier si l'un des joueurs a gagné ou si la partie est nulle
    return true;
}

// Méthode pour afficher le gagnant
void Game::displayWinner() const {
    // Implémenter la logique pour afficher le gagnant ici
}


void Game::displayPlayers() const {
    for (const Player& player : players) {
        std::cout << player << std::endl;
        
    }
}

void Game::playTurn(Player& currentPlayer) {
    

    // Afficher les pièces du joueur
    currentPlayer.displayPieces();

    // Placer trois pièces sur le plateau
    // Demander au joueur de choisir une pièce
    int pieceIndex;
    char pieceLetter;

    while (true) {
        // Demander au joueur de choisir une pièce
        std::cout << "Choose a piece (enter the letter and index): ";
        std::cin >> pieceLetter >> pieceIndex;

        if (std::cin.fail()) {
            std::cin.clear();  // Réinitialiser l'état de cin après une erreur
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignorer le reste de la ligne
            std::cout << "Invalid input. Saisie fausse recommencer" << std::endl;
            continue;
        } 

        // Vérifier si la lettre correspond au type d'animal du joueur
        if (pieceLetter != currentPlayer.getAnimalType()) {
            std::cout << "Invalid choice. Piece type does not match your animal type." << std::endl;
            continue;  // Revenir au début de la boucle
        }

        // Vérifier si l'index est compris entre 1 et 3
        if (pieceIndex < 0 || pieceIndex > 2) {
            std::cout << "Invalid choice. Please enter an index between 0 and 2." << std::endl;
            continue;  // Revenir au début de la boucle
        }

        break;

        // Si toutes les conditions sont remplies, la boucle s'arrête
    } // La condition ici peut être ajustée selon vos besoins


    // Demander au joueur de choisir des coordonnées
    int newX, newY;
    do {
        std::cout << "Enter the coordinates to place the piece (x y): ";
        std::cin >> newX >> newY;

        if (newX < 0 || newX >= Board::boardSize || newY < 0 || newY >= Board::boardSize) {
            // Afficher un message d'erreur si les coordonnées ne sont pas valides
            std::cout << "Invalid coordinates. Please enter values between 0 and " << Board::boardSize - 1 << "." << std::endl;
        }

    } while ((newX < 0 || newX >= Board::boardSize || newY < 0 || newY >= Board::boardSize || board.isPositionOccupied(newX, newY)));

    std::cout << "(dans game) les coord :" << newX << " , " << newY << std::endl;

    currentPlayer.movePiece(pieceIndex, newX, newY);

    // Mettre à jour le plateau
    board.MAJPiece(*currentPlayer.getPiece(pieceIndex),newX, newY);

    // Création d'une instance de Barriere
    placeBarriers();
}

void Game::placeBarriers() {
    do {
    int barrier1X, barrier1Y;
    char lettre;

    // Demander à l'utilisateur de saisir les coordonnées pour la première barrière
    std::cout << "Enter coordinates for the first barrier (x y): ";
    std::cin >> barrier1X >> barrier1Y;

    // Vérifier la validité des coordonnées
    while (barrier1X < 0 || barrier1X >= Board::boardSize || barrier1Y < 0 || barrier1Y >= Board::boardSize) {
        std::cout << "Invalid coordinates. Please enter values between 0 and " << Board::boardSize - 1 << "." << std::endl;
        std::cout << "Enter coordinates for the first barrier (x y): ";
        std::cin >> barrier1X >> barrier1Y;
    }

    /*
    // Demander à l'utilisateur de saisir les coordonnées pour la deuxième barrière
    std::cout << "Enter coordinates for the second barrier (x y): ";
    std::cin >> barrier2X >> barrier2Y;

    // Vérifier la validité des coordonnées
    while (barrier2X < 0 || barrier2X >= Board::boardSize || barrier2Y < 0 || barrier2Y >= Board::boardSize ||
        !(board.isAdjacent(barrier1X, barrier1Y, barrier2X, barrier2Y))) {
        std::cout << "Invalid coordinates. Please enter values between 0 and " << Board::boardSize - 1
                << " and ensure the second barrier is adjacent to the first one." << std::endl;
        std::cout << "Enter coordinates for the second barrier (x y): ";
        std::cin >> barrier2X >> barrier2Y;
    }

    if (board.isBarrierBetween(barrier2X, barrier2Y, barrier1X, barrier1Y)) {
        std::cout << "Inverse barrier already exists. Cannot place the new barrier." << std::endl;
    } else {
        // Placer les barrières sur le plateau
        Barriere barriere ;
        board.addBarrier(barriere);
        std::cout << "Après barrières :" << std::endl << board << std::endl;
    }
    */

    std::cout << "Choisir ou placer la barrière : pour en haut (H), en bas (B), à gauche (G) et à droite (D)";
    std::cin >> lettre;

    board.addBarrier(barrier1X, barrier1Y,lettre);
    
    board.afficherBarrieres();
    
    }while(true);


}
