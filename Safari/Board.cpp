#include "Board.hpp"
#include <iomanip>

// Constructeur
Board::Board() {
    // Initialise le plateau à vide
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            board[i][j].setPiece(nullptr);
        }
    }
}

// Méthode pour gérer la disposition des pièces
void Board::placePiece(int x,int y,Piece& piece) {
    board[x][y].setPiece(&piece);
}


void Board::MAJPiece(Piece& piece, int newX, int newY) {
    int playerID = piece.getPlayerID();
    int pieceID = piece.getPieceID();

    // Parcourir le tableau
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            // Vérifier si la pièce correspond
            if (board[i][j].getPiece() != nullptr &&
                board[i][j].getPiece()->getPlayerID() == playerID &&
                board[i][j].getPiece()->getPieceID() == pieceID) {
                // Mettre à jour la pièce
                board[i][j].setPiece(nullptr);
                board[newX][newY].setPiece(&piece);
                return;  // Sortir de la boucle dès que la pièce est mise à jour
            }
        }
    }
}


bool Board::isPositionOccupied(int x, int y) const {
    if (board[x][y].getPiece() != nullptr)
        std::cout << "Vos coordonées ne sont pas bonnes car la case est déjà occupé" << std::endl;
    return (board[x][y].getPiece() != nullptr);
}



void Board::addBarrier(int x, int y, char L) {
    /* DABORD VERIFIER QUE LA BARRI7RE EST PAS DEJA TRUE + FAIRE LE DOUBLE TRUE */
    switch (L) {
            case 'H':
                board[x][y].getBarriers()->setHaut(true);
                break;
            case 'B':
                board[x][y].getBarriers()->setBas(true);
                break;
            case 'G':
                board[x][y].getBarriers()->setGauche(true);
                break;
            case 'D':
                board[x][y].getBarriers()->setDroite(true);
                break;
            default:
                std::cout << "Direction non valide." << std::endl;
        }
}

bool Board::isBarrierBetween(int x1, int y1, int x2, int y2) const {
    
    return false;
}



std::ostream& operator<<(std::ostream& os, const Board& board) {
    // Affiche le plateau avec les pièces et les barrières
    for (int i = 0; i < Board::boardSize; ++i) {

        for (int j = 0; j < Board::boardSize; ++j) {
            if(board[i][j].getBarriers()->getHaut()==true){
                os << "_____" ;
                
            }
            else{
                os << "    ";
            }

        }
        os << std::endl;

        for (int j = 0; j < Board::boardSize; ++j) {

            Piece* piece = nullptr;

            if (board[i][j].getPiece()!= nullptr) {
                    piece = board[i][j].getPiece();
                
            }

            if (piece != nullptr && board[i][j].getBarriers()->getDroite()==true) {
                os << "[" << std::setw(1) << piece->to_string() << "]|";
            }
            else if (piece != nullptr) {
                os << "[" << std::setw(2) << piece->to_string() << "]";
            }
            else if (board[i][j].getBarriers()->getDroite()==true){
                os << "[" << std::setw(4) << "]|";
            }
            else {
                os << "[" << std::setw(4) << "]";
            }
        }
        os << std::endl;
    }
    return os;
}



bool Board::isAdjacent(int x1, int y1, int x2, int y2) {
    // Votre logique pour vérifier si (x1, y1) et (x2, y2) sont des coordonnées adjacentes
    // Par exemple, vérifier si la distance entre les deux points est égale à 1
    return (std::abs(x1 - x2) == 1 && y1 == y2) || (x1 == x2 && std::abs(y1 - y2) == 1);
}


/*
bool Board::isPieceAt(int x, int y) const {
    // Rechercher la pièce aux coordonnées spécifiées
    auto it = std::find_if(pieces.begin(), pieces.end(), [x, y](const Piece* piece) {
        return piece->getX() == x && piece->getY() == y;
    });

    // Retourner true si la pièce est trouvée, sinon false
    return it != pieces.end();
}



const Piece& Board::getPieceAt(int x, int y) const {
    // Rechercher la pièce aux coordonnées spécifiées
    auto it = std::find_if(pieces.begin(), pieces.end(), [x, y](const Piece& piece) {
        return piece.getX() == x && piece.getY() == y;
    });

    // Retourner la pièce si elle est trouvée, sinon une référence constante par défaut
    return (it != pieces.end()) ? *it : une référence constante par défaut ;
}
*/

void Board::afficherBarrieres() const {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            std::cout << board[i][j].getBarriers();
        }
    }
}




