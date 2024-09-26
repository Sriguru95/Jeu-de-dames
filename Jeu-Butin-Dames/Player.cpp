#include "Player.hpp"

Player::Player(const std::string& playerName) : name{playerName}, score{0}, firstTime{false} {}

Player::~Player() {}

void Player::capturePawn(const Pawn& pawn) {
    pawnsCaptured.push_back(pawn);
    // Mettez à jour le score lors de la capture d'un pion
    score += pawn.getPoints();
}

void Player::capturePawns(const std::vector<Pawn>& capturedPawns) {
    // Ajoute a chaque pion capturé à la liste des pions capturés par le joueur
    for (const auto& pawn : capturedPawns) {
        pawnsCaptured.push_back(pawn);
        // Meise à jour du score pour chaque pion capturé
        score += pawn.getPoints();
    }
}

const std::vector<Pawn>& Player::getPawnsCaptured() const {
    return pawnsCaptured;
}

int Player::getCountCapturedPawns(PawnColor color) const {
    int count = 0;
    for (const auto& pawn : pawnsCaptured) {
        if (pawn.getColor() == color && !pawn.getHasCrown()) {
            count++;
        }
    }
    return count;
}


int Player::calculateScore() const {
    return score;
}

void Player::setScore(int newScore) {
    score = newScore;
}

const std::string& Player::getName() const {
    return name;
}

bool Player::getFirstTime() const {
    return firstTime;
}

void Player::setFirstTime(bool value) {
    firstTime = value;
}

void Player::applyScoreMalus(int malus) {
    // Applique un malus au score
    score -= malus;
}

 void Player::reset() {
    pawnsCaptured.clear(); // Supprime tous les pions capturés
    firstTime = false;      // Réinitialise la variable firstTime à false 
    score = 0;
}

