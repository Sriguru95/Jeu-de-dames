#ifndef BARRIERE_HPP
#define BARRIERE_HPP

#include <utility>

class Barriere {
public:
    // Constructeur
    Barriere();

    // Méthode pour afficher la barrière
    virtual ~Barriere()=default;

    friend std::ostream& operator<<(std::ostream& os, const Barriere& b);

    // Méthodes pour obtenir les coordonnées des cases adjacentes

    void setGauche(bool value);
    void setDroite(bool value); 
    void setHaut(bool value); 
    void setBas(bool value);

    bool getGauche() const;
    bool getDroite() const;
    bool getHaut() const;
    bool getBas() const;


private:
    // Coordonnées des cases adjacentes
    bool gauche;
    bool droite;
    bool haut;
    bool bas;
};

#endif // BARRIERE_HPP
