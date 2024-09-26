#include "Pawn.hpp"

Pawn::Pawn() {
    color = getRandomColor();
    initializePoints();
    hasCrown = false; // Initialisation à false par défaut pas de couronne
}

Pawn::Pawn(PawnColor color) : color{color}, hasCrown{false} {
    initializePoints();
}

Pawn::~Pawn() {}

PawnColor Pawn::getColor() const {
    return color;
}

int Pawn::getPoints() const {
    return points;
}


bool Pawn::getHasCrown() const {
    return hasCrown;
}

void Pawn::setHasCrown(bool value) {
    hasCrown = value;
}


void Pawn::initializePoints() {
    switch (color) {
        case PawnColor::YELLOW:
            points = 1;
            break;
        case PawnColor::RED:
            points = 2;
            break;
        case PawnColor::BLACK:
            points = 3;
            break;
        default:
            points = 1;
    }
}

PawnColor Pawn::getRandomColor() {
    // Génération aléatoire de la couleur
    int randomValue = rand() % 3;
    return static_cast<PawnColor>(randomValue);
}

void Pawn::draw(sf::RenderWindow& window, float x, float y, float size) const {
    // Utilise la taille passée en paramètre pour ajuster le rayon du cercle
    sf::CircleShape pawnCircle(size / 3); 

    // Calcule la position du cercle pour le centrer dans la case
    float centerX = x + size / 2.0f;
    float centerY = y + size / 2.0f;
    pawnCircle.setPosition(centerX - pawnCircle.getRadius(), centerY - pawnCircle.getRadius());

    // Définition de la couleur du cercle en fonction de la couleur du pion
    switch (color) {
        case PawnColor::YELLOW:
            pawnCircle.setFillColor(sf::Color::Yellow);
            break;
        case PawnColor::RED:
            pawnCircle.setFillColor(sf::Color::Red);
            break;
        case PawnColor::BLACK:
            pawnCircle.setFillColor(sf::Color::Black);
            break;
        case PawnColor::WHITE:
            pawnCircle.setFillColor(sf::Color::White);
            break;
    }

    // Dessine la couronne si le pion a une couronne
    if (hasCrown) {
        // Dessine la couronne ici, par exemple, un petit rectangle au-dessus du cercle
        sf::RectangleShape crown(sf::Vector2f(size / 2, size / 10));
        crown.setPosition(centerX - crown.getSize().x / 2, y - size / 10);
        crown.setFillColor(sf::Color::Yellow); 
        window.draw(crown);
    }

    // Dessine le cercle dans la fenêtre
    window.draw(pawnCircle);
}

string Pawn::getColorString() const {
    switch (color) {
        case PawnColor::YELLOW:
            return "Jaune";
        case PawnColor::RED:
            return "Rouge";
        case PawnColor::BLACK:
            return "Noir";
        case PawnColor::WHITE:
            return "Blanc";
        default:
            return "";
    }
}


ostream& operator<<(ostream& os, const Pawn& pawn) {
    os << "Color: ";
    switch (pawn.color) {
        case PawnColor::YELLOW:
            os << "Yellow";
            break;
        case PawnColor::RED:
            os << "Red";
            break;
        case PawnColor::BLACK:
            os << "Black";
            break;
        case PawnColor::WHITE:
            os << "White";
            break;
        
    }
    os << ", Points: " << pawn.points;
    return os;
}

