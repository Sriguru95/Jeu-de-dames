#include <iostream>
#include <vector>
#include <iomanip>
const int BOARD_SIZE = 8;

class Board {
private:
    char board[BOARD_SIZE][BOARD_SIZE];
    bool barriers[BOARD_SIZE - 1][BOARD_SIZE][4]; // 0: haut, 1: gauche, 2: bas, 3: droite

public:
    Board() {
        initializeBoard();
        initializeBarriers();
    }

    // Initialise le plateau au début de la partie
    void initializeBoard() {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                board[i][j] = ' ';
            }
        }
    }

    // Initialise les barrières
    void initializeBarriers() {
        for (int i = 0; i < BOARD_SIZE - 1; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                for (int k = 0; k < 4; ++k) {
                    barriers[i][j][k] = false;
                }
            }
        }
    }

    // Place une barrière dans une direction donnée
    void placeBarrier(int row, int col, int direction) {
        barriers[row][col][direction] = true;
    }

    // Affiche le plateau de jeu avec les barrières
    void displayBoard() const {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                std::cout << "[" << std::setw(1) << "case " << i << "," << j << "]" ;

                if (j < BOARD_SIZE - 1 && barriers[i][j][3]) {
                    std::cout << "|";
                }
            }

            std::cout << std::endl;

            if (i < BOARD_SIZE - 1) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    if (barriers[i][j][2]) {
                        std::cout << "_";
                    } else {
                        std::cout << " ";
                    }

                    if (j < BOARD_SIZE - 1 && barriers[i][j][3]) {
                        std::cout << "|";
                    }
                }
                std::cout << std::endl;
            }
        }
    }

    // Vérifie si la partie est terminée
    bool isGameOver() const {
        // Implémentez la logique pour vérifier les conditions de fin de partie
        // Retournez true si la partie est terminée, sinon false
        // Vous pouvez ajouter votre logique ici en fonction de vos règles spécifiques
        return false;
    }
};

int main() {
    Board gameBoard;
    std::cout << "plateau" << std::endl;
    gameBoard.placeBarrier(1, 2, 0);  // Place une barrière en haut de la case (1, 2)
    gameBoard.placeBarrier(3, 4, 1);  // Place une barrière à gauche de la case (3, 4)
    gameBoard.displayBoard();

    // Votre logique de jeu peut aller ici

    return 0;
}
