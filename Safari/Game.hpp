#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Board.hpp"
#include "Rhinoceros.hpp"
#include "Lion.hpp"
#include "Elephant.hpp"

class Game {
public:
    // Constructeur
    Game();

    // Méthode pour démarrer la partie
    void start();

private:
    // Nombre de joueurs
    static const int numPlayers = 3;

    // Tableau des joueurs
    Player players[numPlayers] = {Player(-1, 'N'), Player(-1, 'N'), Player(-1, 'N')};

public:

    // Plateau de jeu
    Board board;

    // Méthode pour gérer les tours des joueurs
    void mettrePiece();

    // Méthode pour vérifier la fin de la partie
    bool isGameOver() const;

    // Méthode pour afficher le gagnant
    void displayWinner() const;

    void displayPlayers() const;

    void placeBarriers();

    void playTurn(Player& currentPlayer);

    virtual ~Game()=default;
    
};

#endif // GAME_HPP