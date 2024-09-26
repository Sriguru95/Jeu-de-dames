#ifndef PAWN_H
#define PAWN_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random> // pour la génération aléatoire des couleurs

using namespace std;

enum class PawnColor {
    YELLOW,
    RED,
    BLACK,
    WHITE
};

class Pawn {
private:
    PawnColor color;
    int points;
    bool hasCrown;

public:
    // Constructeurs
    Pawn();
    explicit Pawn(PawnColor color);

    // Destructeur
    ~Pawn();

    // Accesseurs
    PawnColor getColor() const;
    int getPoints() const;

    string getColorString() const;

    // Accesseurs pour hasCrown
    bool getHasCrown() const;
    void setHasCrown(bool value);

    // Méthode pour initialiser les points en fonction de la couleur
    void initializePoints();

    // Méthode pour obtenir une couleur au hasard
    static PawnColor getRandomColor();

    // Méthode pour afficher le pion graphiquement (SFML)
    void draw(sf::RenderWindow& window, float x, float y, float size) const;

    // Surcharge de l'opérateur de flux de sortie (Affichage)
    friend ostream& operator<<(ostream& os, const Pawn& pawn);

};

#endif // PAWN_H
