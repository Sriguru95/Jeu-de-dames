#include "Case.hpp"
#include <iomanip>

Case::Case() : piece(nullptr),barrieres(new Barriere) {
}

std::ostream& operator<<(std::ostream& os, const Case& c) {
        if (c.piece!=nullptr)
            os << "[" << std::setw(1) << *c.piece << "]";
        else 
            os << "[" << std::setw(2) << "]";
        return os;
}

void Case::setPiece(Piece* newPiece) {
    piece = newPiece;
}

void Case::setBarrier(Barriere* newBarrier) {
    barrieres = newBarrier;
}

Piece* Case::getPiece() const {
    return piece;
}

Barriere* Case::getBarriers() const {
    return barrieres;
}


