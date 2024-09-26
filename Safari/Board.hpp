#ifndef BOARD_HPP
#define BOARD_HPP

#include "Piece.hpp"
#include "Barriere.hpp"
#include "Case.hpp"
#include <vector>
#include <ostream>

class Board {
public:
    // Constructeur
    Board();

    // Méthodes pour gérer la disposition des pièces
    void placePiece(int x,int y,Piece& piece);
    void MAJPiece(Piece& piece,int newX, int newY);
    bool isPositionOccupied(int x, int y) const;

    // Méthodes pour gérer l'ajout de barrières
    void addBarrier(int x, int y, char L);
    bool isBarrierBetween(int x1, int y1, int x2, int y2) const;

    // Opérateur de flux de sortie
    friend std::ostream& operator<<(std::ostream& os, const Board& board);

    bool isAdjacent(int x1, int y1, int x2, int y2);
    virtual ~Board()=default;

    void afficherBarrieres() const;

    const Case* operator[](int index) const {
        return board[index];
    }

    // Opérateur de tableau pour accéder aux cases du plateau
    Case* operator[](int index) {
        return board[index];
    }


    /*bool isPieceAt(int x, int y) const;
    const Piece& getPieceAt(int x, int y) const ;
    bool hasBarrierAt(int x, int y) const;*/

public:
    // Dimensions du plateau
    static const int boardSize = 8;
private:
    // Matrice pour représenter le plateau (8x8)
    Case board[boardSize][boardSize];

};

#endif // BOARD_HPP