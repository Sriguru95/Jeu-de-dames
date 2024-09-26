#include "Board.hpp"

Board::Board(int rows, int cols, const sf::Vector2u& windowSize) : rows{rows}, cols{cols}, squareSize{0.0f} {
    // Initialisation du générateur de nombres aléatoires
    srand(static_cast<unsigned>(time(nullptr)));

    // initialise une grille de cases (Square) avec rows lignes et cols colonnes, où chaque case a initialement des coordonnées row et col de 0.
    squares.resize(rows, vector<Square>(cols, Square(0, 0)));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            squares[i][j] = Square(i, j);
        }
    }

    // Calcule la taille des cases en fonction de la taille de la fenêtre
    squareSize = std::min(static_cast<float>(windowSize.x) / cols, static_cast<float>(windowSize.y) / rows);
}

Board::~Board() {}

int Board::getRows() const {
    return rows;
}

int Board::getCols() const {
    return cols;
}

float Board::getSquareSize() const {
    return squareSize;
}

Square& Board::getSquare(int row, int col) {
    // Vérifie les limites de l'indice pour éviter les débordements
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        return squares[row][col];
    } else {
        // Gestion explicite d'une case "hors plateau"
        cerr << "Erreur : Tentative d'accès à une case hors plateau." << endl;
        // Retourne une référence à une case hors plateau 
        return squares[0][0];
    }
}

Square& Board::getSelectedSquare() {
    // Récupére la case sélectionnée
    Square* selectedSquare = nullptr;
    for (int i = 0; i < getRows() && !selectedSquare; ++i) {
        for (int j = 0; j < getCols() && !selectedSquare; ++j) {
            if (getSquare(i, j).getSelected()) {
                selectedSquare = &getSquare(i, j);
            }
        }
    }
    if (!selectedSquare) {
        // Gérer le cas où aucune case n'est sélectionnée
        static Square dummySquare;  // Case bidon ou spéciale
    
        return dummySquare;
    }
    return *selectedSquare;
}

void Board::setSquareSelected(int row, int col, bool selected) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        squares[row][col].setSelected(selected);
    }
}

void Board::setSquareColor(int row, int col, SquareColor color) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        squares[row][col].setCurrentColor(color);
    }
}


void Board::placePawn(int row, int col, const Pawn& pawn) {
    // Place le pion sur la case spécifiée
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        squares[row][col].placePawn(pawn);
    }
}

void Board::removePawnAt(int row, int col) {
    // Retire le pion de la case spécifiée
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        squares[row][col].removePawn();
    }
}

// Configuration du plateau du jeu de Butin
void Board::setupButinGame() {
    // Génération des pions
    vector<Pawn> pawns;

    // Génération des pions jaunes
    for (int i = 0; i < 34; ++i) {
        pawns.push_back(Pawn(PawnColor::YELLOW));
    }

    // Génération des pions rouges
    for (int i = 0; i < 20; ++i) {
        pawns.push_back(Pawn(PawnColor::RED));
    }

    // Génération des pions noirs
    for (int i = 0; i < 10; ++i) {
        pawns.push_back(Pawn(PawnColor::BLACK));
    }

    // Mélange aléatoire des positions pour l'ensemble du plateau
    vector<pair<int, int>> positions;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            positions.push_back({i, j});
        }
    }

    random_shuffle(positions.begin(), positions.end());

    // Placement des pions sur le plateau
    int pawnIndex = 0;
    for (size_t i = 0; i < pawns.size(); ++i) {
        int row = positions[i].first;
        int col = positions[i].second;
        placePawn(row, col, pawns[pawnIndex]);
        ++pawnIndex;
    }
}

// Configuration du plateau du jeu de Dames
void Board::setupDamesGame() {
    // Place les cases marron clair et marron foncé sur le damier
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Alterne les couleurs en fonction de la position sur le damier
            if ((i + j) % 2 == 0) {
                squares[i][j].setCurrentColor(SquareColor::LIGHT_BROWN);
            } else {
                squares[i][j].setCurrentColor(SquareColor::DARK_BROWN);
            }
        }
    }

    // Place les pions sur les cases de couleur foncée en partant du haut
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Place les pions noirs sur les quatre premières lignes de cases foncées 
            if (i < 4 && squares[i][j].getCurrentColor() == SquareColor::DARK_BROWN) {
                // Place un pion noir sur la case
                placePawn(i, j, Pawn(PawnColor::BLACK));
            }
            // Place les pions blanc sur les quatre dernières lignes de cases foncées 
            else if (i >= rows - 4 && squares[i][j].getCurrentColor() == SquareColor::DARK_BROWN) {
                // Place un pion blanc sur la case
                placePawn(i, j, Pawn(PawnColor::WHITE));
            }
        }
    }
}


// On dessine le plateau
void Board::draw(sf::RenderWindow& window) const {
    // Dessine chaque case du plateau
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = j * squareSize;  
            float y = i * squareSize; 
            squares[i][j].draw(window, x, y, squareSize);

            // Ajoute les lignes de la grille
            sf::RectangleShape line(sf::Vector2f(cols * squareSize, 2));
            line.setPosition(0, i * squareSize);
            line.setFillColor(sf::Color::Black);
            window.draw(line);

            line.setSize(sf::Vector2f(2, rows * squareSize));
            line.setPosition(j * squareSize, 0);
            window.draw(line);
        }
    }
}

// On reinitialise les couleurs des cases (jeu de butin)
void Board::resetSquareColors() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if(squares[i][j].getCurrentColor() != SquareColor::BLUE) {
                squares[i][j].setSelected(false);
                squares[i][j].setCurrentColor(SquareColor::WHITE);
            }
        }
    }
}

// On reinitialise les couleurs des cases (jeu de dames)
void Board::resetSquareColorsDames() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if(squares[i][j].getCurrentColor() == SquareColor::CYAN) {
                squares[i][j].setSelected(false);
                squares[i][j].setCurrentColor(SquareColor::DARK_BROWN);
            }
            if(squares[i][j].getCurrentColor() == SquareColor::GREEN) {
                squares[i][j].setSelected(false);
                squares[i][j].setCurrentColor(SquareColor::DARK_BROWN);
            }
            if(squares[i][j].getCurrentColor() == SquareColor::WHITE) {
                squares[i][j].setSelected(false);
                squares[i][j].setCurrentColor(SquareColor::DARK_BROWN);
            }
        }
    }
}

// On recupere les cases adjacents d'un pion (jeu de butin)
std::vector<sf::Vector2i> Board::getAdjacentPawnPositions(const sf::Vector2i& position) {
    std::vector<sf::Vector2i> adjacentPositions;

    // Définition des directions possibles (haut, bas, gauche, droite, diagonales)
    const std::vector<sf::Vector2i> directions = {
        {0, -1}, // Haut
        {0, 1},  // Bas
        {-1, 0}, // Gauche
        {1, 0},  // Droite
        {-1, -1}, // Diagonale haut-gauche
        {1, -1},  // Diagonale haut-droite
        {-1, 1},  // Diagonale bas-gauche
        {1, 1}    // Diagonale bas-droite
    };

    // Parcoure les directions et ajoutez les positions adjacentes
    for (const auto& direction : directions) {
        int row = position.x + direction.x;
        int col = position.y + direction.y;

        // Vérifie les limites de l'indice pour éviter les débordements
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            adjacentPositions.push_back({row, col});
        }
    }

    return adjacentPositions;
}


void Board::highlightPossibleJumps() {
    //  la case sélectionnée
    Square& selectedSquare = getSelectedSquare();

    // la position de la case sélectionnée
    sf::Vector2i selectedPosition(selectedSquare.getRow(), selectedSquare.getCol());

    // les positions adjacentes
    std::vector<sf::Vector2i> adjacentPositions = getAdjacentPawnPositions(selectedPosition);

    // Parcourt des positions adjacentes et modifie la couleur des cases
    for (const auto& pos : adjacentPositions) {
        int row = pos.x;
        int col = pos.y;

        // Vérifie les limites de l'indice pour éviter les débordements
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            // On recupere sa case adjacente
            Square& adjacentSquare = squares[row][col];

            // Vérifie si sa case adjacente contient un pion et qu'il n'est pas deja sauté
            if (adjacentSquare.getPawn() != nullptr && adjacentSquare.getCurrentColor() != SquareColor::BLUE) {
                // On recupere la position suivante dans la même direction
                int nextRow = row + (row - selectedPosition.x);
                int nextCol = col + (col - selectedPosition.y);

                // Vérifie les limites de l'indice pour éviter les débordements
                if (nextRow >= 0 && nextRow < rows && nextCol >= 0 && nextCol < cols) {
                    // On recupere la case suivante
                    Square& nextSquare = squares[nextRow][nextCol];

                    // Vérifie si la case suivante est libre
                    if (nextSquare.getPawn() == nullptr) {
                        // Modifie la couleur de la case suivante en vert
                        nextSquare.setCurrentColor(SquareColor::GREEN);
                    }
                }
            }
        }
    }
}


std::vector<Pawn> Board::getJumpedPawns() const {
    std::vector<Pawn> jumpedPawns;

    // Parcourt toutes les cases du plateau
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // On recupere la couleur actuelle de la case
            SquareColor currentColor = squares[i][j].getCurrentColor();

            // Vérifie si la case est colorée en bleu (sautée)
            if (currentColor == SquareColor::BLUE) {
                // Ajoute le pion sauté à la liste
                jumpedPawns.push_back(*squares[i][j].getPawn());
            }
        }
    }

    return jumpedPawns;
}


void Board::removeJumpedPawns() {
    // Parcourt toutes les cases du plateau
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // On recupere la couleur actuelle de la case
            SquareColor currentColor = squares[i][j].getCurrentColor();

            // Vérifie si la case est colorée en bleu (sautée)
            if (currentColor == SquareColor::BLUE) {
                // Retire le pion de la case
                squares[i][j].removePawn();
            
                // Réinitialise la couleur de la case à blanc
                squares[i][j].setCurrentColor(SquareColor::WHITE);
            }
        }
    }
}

// verifier la fin de partie du jeu de butin
bool Board::canYellowPawnsJump() {
    // Parcourt toutes les cases du plateau
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Vérifie si la case contient un pion jaune
            if (squares[i][j].getPawn() != nullptr && squares[i][j].getPawn()->getColor() == PawnColor::YELLOW) {
                // On recupere la position du pion jaune
                sf::Vector2i currentPosition(i, j);

                // On recupere les positions adjacentes
                std::vector<sf::Vector2i> adjacentPositions = getAdjacentPawnPositions(currentPosition);

                // Parcourt les positions adjacentes et vérifie si des sauts sont possibles
                for (const auto& pos : adjacentPositions) {
                    int row = pos.x;
                    int col = pos.y;

                    // Vérifie les limites de l'indice pour éviter les débordements
                    if (row >= 0 && row < rows && col >= 0 && col < cols) {
                        // On recupere la case adjacente
                        const Square& adjacentSquare = squares[row][col];

                        // Vérifie si la case adjacente contient un pion et qu'il n'est pas déjà sauté
                        if (adjacentSquare.getPawn() != nullptr && adjacentSquare.getCurrentColor() != SquareColor::BLUE) {
                            // On recupere la position suivante dans la même direction
                            int nextRow = row + (row - currentPosition.x);
                            int nextCol = col + (col - currentPosition.y);

                            // Vérifie les limites de l'indice pour éviter les débordements
                            if (nextRow >= 0 && nextRow < rows && nextCol >= 0 && nextCol < cols) {
                                // On recupere la case suivante
                                const Square& nextSquare = squares[nextRow][nextCol];

                                // Vérifie si la case suivante est libre
                                if (nextSquare.getPawn() == nullptr) {
                                    return true;  // Des sauts sont possibles
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return false;  // Aucun saut possible
}

void Board::removeRemainingPawns() {
    // Parcourt toutes les cases du plateau
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Vérifie si la case contient un pion
            if (squares[i][j].getPawn() != nullptr) {
                // Retire le pion de la case
                squares[i][j].removePawn();
            }
        }
    }
}

int Board::calculateRemainingPawnsPoints() const {
    int malus = 0;

    // Parcourt toutes les cases du plateau
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Vérifie si la case contient un pion
            if (squares[i][j].getPawn() != nullptr) {
                // Ajoute les points du pion au malus
                malus += squares[i][j].getPawn()->getPoints();
            }
        }
    }

    return malus;
}

bool Board::remainingMouvments() const {
    // Parcourt toutes les cases du plateau
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if(squares[i][j].getCurrentColor() == SquareColor::GREEN){ // si il y a encore des case verte alors il y a encore des mouvements possible
                return true;
            }
        }
    }
    return false;
}