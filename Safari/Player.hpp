#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Piece.hpp"
#include <vector>

class Player {
public:
    // Constructeur
    Player(int playerId, char animalType);

    int getID() const;

    // Méthode pour afficher les pièces du joueur
    void displayPieces() const;

    void displayPiecesWithCoordinates() const;

    // Méthodes pour gérer les actions du joueur
    void movePiece(int pieceIndex, int newX, int newY);
    void placeBarrier(int x1, int y1, int x2, int y2);
    void placePiece(int pieceIndex, int newX, int newY);

    // Méthode pour vérifier si le joueur a gagné
    bool hasWon() const;

    friend std::ostream& operator<<(std::ostream& os, const Player& player);

    int getNumPieces() const;

    char getAnimalType() const;

    Piece* getPiece(int pieceIndex) const;

    virtual ~Player()=default;


private:
    // Identifiant du joueur
    int id;

    // Type d'animal (R, L, E)
    char animalType;

    // Vecteur pour stocker les pièces du joueur
    std::vector<Piece*> pieces;  // Utilisation de pointeurs pour gérer des objets polymorphes
};

#endif // PLAYER_HPP
