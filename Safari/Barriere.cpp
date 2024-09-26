#include "Barriere.hpp"
#include <iostream>

// Constructeur
Barriere::Barriere() : gauche{false}, droite{false}, haut{false}, bas(false){

}

// Méthode pour afficher la barrière
std::ostream& operator<<(std::ostream& os, const Barriere& b) {
    os << "Barriere : haut " << b.haut << " bas " << b.bas << " gauche " << b.gauche << " droite " << b.droite << std::endl;
    return os;
}

void Barriere::setGauche(bool value) {
        gauche = value;
}

void Barriere::setDroite(bool value) {
        droite = value;
}

void Barriere::setHaut(bool value) {
        haut = value;
}

void Barriere::setBas(bool value) {
        bas = value;
}

bool Barriere::getGauche() const {
        return gauche ;
}

bool Barriere::getDroite() const{
        return droite ;
}

bool Barriere::getHaut() const{
        return haut;
}

bool Barriere::getBas() const{
        return bas ;
}