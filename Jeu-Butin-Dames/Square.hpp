#ifndef SQUARE_H
#define SQUARE_H

#include <SFML/Graphics.hpp>
#include "Pawn.hpp"

enum class SquareColor {
    WHITE,
    GREEN,  
    CYAN,
    BLUE,
    LIGHT_BROWN,
    DARK_BROWN
};

class Square {
private:
    int row;  // Numéro de ligne
    int col;  // Numéro de colonne
    Pawn* pawn;  // Pion actuellement sur la case (peut être nullptr s'il n'y a pas de pion)
    SquareColor currentColor;

    bool isSelected;

public:
    // Constructeurs
    Square();
    Square(int row, int col);
   

    // Destructeur
    ~Square();

    // Accesseurs
    int getRow() const;
    int getCol() const;
    const Pawn* getPawn() const;

    // Méthode pour placer un pion sur la case
    void placePawn(const Pawn& newPawn);

    // Méthode pour retirer un pion de la case
    void removePawn();

    // Méthode pour afficher graphiquement la case avec SFML
    void draw(sf::RenderWindow& window, float x, float y, float size) const;

    void setSelected(bool selected);
    bool getSelected() const;

    void setCurrentColor(SquareColor color);
    SquareColor getCurrentColor() const;
};

#endif 