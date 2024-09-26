#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Pawn.hpp"

class Player {
private:
    std::vector<Pawn> pawnsCaptured;
    std::string name;
    int score;  

    bool firstTime; // 1ère fois que le joueur joue (utile pour la phase de preparation jeu de butin)

public:
    // Constructeur
    Player(const std::string& playerName);

    // Destructeur
    ~Player();

    // Méthode pour capturer un pion
    void capturePawn(const Pawn& pawn);

    // Méthode pour capturer des pions
    void capturePawns(const std::vector<Pawn>& capturedPawns);

    // Méthode pour obtenir les pions capturés
    const std::vector<Pawn>& getPawnsCaptured() const;

    // Méthode pour calculer le score du joueur en fonction des pions capturés
    int calculateScore() const;

    void setScore(int newScore);

    // Méthode pour obtenir le nom du joueur
    const std::string& getName() const;

    bool getFirstTime() const;

    void setFirstTime(bool value);

    int getCountCapturedPawns(PawnColor color) const;

    // Méthode pour appliquer un malus au score
    void applyScoreMalus(int malus);

    void reset();
};

#endif
