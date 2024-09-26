#include "Piece.hpp"

// Constructeur
Piece::Piece(char symbol, int playerID,int pieceID)
    : symbol{symbol}, playerID{playerID},pieceID{pieceID} {}

// Méthode pour afficher l'animal
std::ostream& operator<<(std::ostream& os, const Piece& piece) {
        os << piece.symbol << piece.pieceID ;
        return os;
}

void Piece::display() {
        std::cout << symbol << pieceID << std::endl;
}
std::string Piece::to_string() const {
        return std::string(1, symbol) + std::to_string(pieceID);
}


// Méthode pour obtenir le symbole de l'animal
char Piece::getSymbol() const {
    return symbol;
}

// Méthode pour obtenir l'ID du joueur propriétaire
int Piece::getPlayerID() const {
    return playerID;
}


int Piece::getPieceID() const {
    return pieceID;
}


// Destructeur virtuel pour permettre la polymorphie
