#include "Square.hpp"

Square::Square() : row{0}, col{0}, pawn{nullptr}, currentColor{SquareColor::WHITE}, isSelected{false} {}

Square::Square(int row, int col) : row{row}, col{col}, pawn{nullptr}, currentColor{SquareColor::WHITE}, isSelected{false} {}

Square::~Square() {
    // Si un pion est présent, libérez la mémoire
    if (pawn != nullptr) {
        delete pawn;
    }
}

int Square::getRow() const {
    return row;
}

int Square::getCol() const {
    return col;
}

const Pawn* Square::getPawn() const {
    return pawn;
}

void Square::placePawn(const Pawn& newPawn) {
    // Si la case a déjà un pion, on retire d'abord
    if (pawn != nullptr) {
        removePawn();
    }

    // Créer une copie du pion et on place sur la case
    pawn = new Pawn(newPawn);
}

void Square::removePawn() {
    // Si un pion est présent, on libére la mémoire
    if (pawn != nullptr) {
        delete pawn;
        pawn = nullptr;

        isSelected = false;
    }
}

void Square::draw(sf::RenderWindow& window, float x, float y, float size) const {
    // Utilise la taille passée en paramètre pour ajuster la taille du rectangle
    sf::RectangleShape squareRect(sf::Vector2f(size, size));  // Ajustez la taille de la case selon votre logique

    // Définition de la position du rectangle dans la fenêtre
    squareRect.setPosition(x, y);

    // Modifie la couleur en fonction de l'état de surbrillance
    switch (currentColor) {
        case SquareColor::WHITE:
            squareRect.setFillColor(sf::Color::White);
            break;
        case SquareColor::GREEN:
            squareRect.setFillColor(sf::Color::Green);
            break;
        case SquareColor::CYAN:
            squareRect.setFillColor(sf::Color::Cyan);
            break;
        case SquareColor::BLUE:
            squareRect.setFillColor(sf::Color::Blue);
            break;
        case SquareColor::LIGHT_BROWN:
            squareRect.setFillColor(sf::Color(245, 222, 179)); 
            break;
        case SquareColor::DARK_BROWN:
            squareRect.setFillColor(sf::Color(218, 160, 109));  
            break;
    }

    // Dessine la case dans la fenêtre
    window.draw(squareRect);

    // Dessine le pion s'il y en a un sur la case
    if (pawn != nullptr) {
        // calculs pour centrer le pion dans la case
        float pawnX = x + (size - size / 1.5) / 2.0f;  
        float pawnY = y + (size - size / 1.5) / 2.0f;  
        pawn->draw(window, pawnX, pawnY, size / 1.5);
    }
}


void Square::setSelected(bool selected) {
    isSelected = selected;
}

bool Square::getSelected() const {
    return isSelected;
}

void Square::setCurrentColor(SquareColor color) {
    currentColor = color;
}

SquareColor Square::getCurrentColor() const {
    return currentColor;
}