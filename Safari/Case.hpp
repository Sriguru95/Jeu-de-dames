#ifndef CASE_HPP
#define CASE_HPP

#include "Piece.hpp"
#include "Barriere.hpp"
#include <array>

class Case {
private:
    Piece* piece; 
    Barriere* barrieres;

public:
    Case();
    Piece* getPiece() const;
    Barriere* getBarriers() const;
    void setPiece(Piece* newPiece);
    void setBarrier(Barriere* newBarrier);
    friend std::ostream& operator<<(std::ostream& os, const Case& c);
    virtual ~Case()=default;
};

#endif // CASE_HPP
