#ifndef PIECE_HPP
#define PIECE_HPP

#include <iostream>

class Piece {
    private:
        char symbol;  // Symbole représentant l'animal sur le plateau
        int playerID; // ID du joueur propriétaire de l'animal
        int pieceID;

    public:
        // Constructeur
        Piece(char symbol, int playerID,int pieceID);

        // Méthode pour afficher l'animal
        friend std::ostream& operator<<(std::ostream& os, const Piece& piece);
        void display();
        std::string to_string() const;

        // Méthode pour obtenir le symbole de l'animal
        char getSymbol() const;

        // Méthode pour obtenir l'ID du joueur propriétaire
        int getPlayerID() const;

        int getPieceID() const;

        // Destructeur virtuel pour permettre la polymorphie
        virtual ~Piece()=default;

};

#endif // PIECE_HPP
