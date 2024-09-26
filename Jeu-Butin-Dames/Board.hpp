#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <algorithm>  // Pour random_shuffle
#include <ctime>      // Pour initialiser le générateur de nombres aléatoires
#include "Square.hpp"

class Board {
private:
    int rows;
    int cols;
    vector<vector<Square>> squares;

    float squareSize;

public:
    // Constructeurs et destructeurs
    Board(int rows, int cols, const sf::Vector2u& windowSize);
    ~Board();

    // Accesseurs
    int getRows() const;
    int getCols() const;
    Square& getSelectedSquare();
    float getSquareSize() const;
    vector<sf::Vector2i> getAdjacentPawnPositions(const sf::Vector2i& position);
    vector<Pawn> getJumpedPawns() const;

    // Méthode pour accéder à une case spécifique
    Square& getSquare(int row, int col); 

    // Méthode pour placer un pion sur une case spécifique
    void placePawn(int row, int col, const Pawn& pawn);

    // Méthode pour retirer un pion d'une case spécifique
    void removePawnAt(int row, int col);

    // Méthode pour configurer le plateau de jeu avec les pions au hasard (jeu de butin)
    void setupButinGame();

    // Méthode pour configurer le plateau de jeu dames
    void setupDamesGame();

    // Méthode pour afficher graphiquement le damier avec SFML
    void draw(sf::RenderWindow& window) const;

    // mettre les cases en vert des sauts possibles
    void highlightPossibleJumps();

    void resetSquareColors();
    void resetSquareColorsDames();

    void setSquareSelected(int row, int col, bool selected);
    void setSquareColor(int row, int col, SquareColor color);

    void removeJumpedPawns();
    void removeRemainingPawns();

    bool canYellowPawnsJump();
    int calculateRemainingPawnsPoints() const;
    bool remainingMouvments() const;



};

#endif 
