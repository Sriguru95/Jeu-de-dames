#include "Player.hpp"
#include "Rhinoceros.hpp"
#include "Lion.hpp"  
#include "Elephant.hpp" 
#include "Board.hpp" 
#include "Game.hpp" 
#include <iostream>

// Constructeur
Player::Player(int playerId, char animalType) : id(playerId), animalType(animalType) {
    // Initialisation des pièces du joueur
    for (int i = 0; i < 3; ++i) {
        if (animalType == 'R') {
            pieces.push_back(new Rhinoceros(playerId,i));
        }
        else if (animalType == 'L') {
            pieces.push_back(new Lion(playerId,i));
        }
        else if (animalType == 'E') {
            pieces.push_back(new Elephant(playerId,i));
        }
        
    }
}


std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "Joeur " << player.id << " ( " << player.animalType << " ) " << " Pieces:" << std::endl;
    for (const auto& piece : player.pieces) {
        piece->display() ;
    }
    return os;
}


/*
// Méthode pour déplacer une pièce du joueur
void Player::movePiece(int pieceIndex, int newX, int newY) {
    std::cout << "(dans player) les coord :" << newX << " , " << newY << std::endl;
    pieces[pieceIndex]-> move(newX, newY);

    Piece* selectedPiece = getPiece(pieceIndex);
    selectedPiece->setCoordinates(newX, newY);

}

// Méthode pour placer une barrière
void Player::placeBarrier(int x1, int y1, int x2, int y2) {
    // Vous pouvez ajouter des vérifications supplémentaires ici si nécessaire
    pieces.push_back(new Piece(x1, y1, 'B'));  // Supposons que 'B' représente une barrière
    pieces.push_back(new Piece(x2, y2, 'B'));

}

void Player::placePiece(int pieceIndex, int newX, int newY) {
    Piece* selectedPiece = getPiece(pieceIndex);

    // Vérifier si la pièce existe et si ses coordonnées sont valides
    if (selectedPiece && newX >= 0 && newX < Board::boardSize && newY >= 0 && newY < Board::boardSize) {
        // Vérifier si les coordonnées de la pièce sont -1, -1
        if (selectedPiece->getX() == -1 && selectedPiece->getY() == -1) {
            // Modifier les coordonnées de la pièce
            selectedPiece->setCoordinates(newX, newY);
        } 
        else {
            std::cout << "Piece is already placed. Choose another piece." << std::endl;
        }
    } 
    else {
        std::cout << "Invalid coordinates. Please enter coordinates between 0 and 8." << std::endl;
    }
}

*/

// Méthode pour vérifier si le joueur a gagné
bool Player::hasWon() const {
    // Vous pouvez ajouter une logique spécifique pour déterminer si le joueur a gagné
    // Par exemple, vérifier si l'une des pièces du joueur a atteint la ligne opposée dans un jeu de dames
    // ou si le joueur a réussi à capturer toutes les pièces adverses, selon les règles de votre jeu.
    return false;
}

int Player::getID() const {
    return id;
}

int Player::getNumPieces() const {
    return pieces.size();
}
char Player::getAnimalType() const {
    return animalType;
}

Piece* Player::getPiece(int pieceIndex) const {
    if (pieceIndex >= 0 && pieceIndex < pieces.size()) {
        return pieces[pieceIndex];
    } else {
        // Retourner nullptr si l'index est invalide
        return nullptr;
    }
}