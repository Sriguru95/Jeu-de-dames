#include "DamesGame.hpp"
#include <iostream>

DamesGame::DamesGame(const std::vector<std::string>& playerNames)
    : board{10, 10, sf::Vector2u(1200, 800)},
      players{},
      currentPlayerIndex{0},
      pawnSelected{false},
      PawnMoved{false},
      gameEnded{false},
      restartRequested{false},
      returnToMenuRequested{false}{

    // Initialisation des joueurs
    for (const auto& playerName : playerNames) {
        players.push_back(Player(playerName));
    }
    
}


void DamesGame::run(sf::RenderWindow& window) {

    window.setTitle("Jeu de Dames");

    createBoard();

    // Boucle principale du jeu
    while (window.isOpen() && !returnToMenuRequested) {
        handleWindowEvents(window);
        updateDisplay(window);
    }
}


bool DamesGame::hasReturnToMenuRequested() const {
        return returnToMenuRequested;
}


void DamesGame::resetGame() {
    // Réinitialise toutes les variables du jeu ici (comme le constructeur)
    currentPlayerIndex = 0;
    pawnSelected = false;
    PawnMoved = false;
    gameEnded = false;
    restartRequested = false;

    // Réinitialise les données de chaque joueur
    for (auto& player : players) {
        player.reset(); 
    }

    createBoard();
}


void DamesGame::createBoard() {
    board.setupDamesGame();
}


void DamesGame::handleWindowEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                
                // Gére le clic de souris sur le plateau
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                handleMouseClick(mousePosition);

                // Gére le clic du bouton "Rejouer"
                if (gameEnded && restartButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))
                    && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    restartRequested = true;
                }
                
                // Gére le clic du bouton "Retour"
                if (backButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))
                    && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    returnToMenuRequested = true;
                    window.setTitle("Jeux de plateau");
                }
           
            }
        }
    }

    // Déroulement du jeu
    mainGameLoop(window);

    // Si un redémarrage est demandé, on réinitialise le jeu
    if (restartRequested) {
        resetGame();
    }

}

// Gestion de la selection d'un pion ou d'une dame via la souris
void DamesGame::selectPawn(const sf::Vector2i& mousePosition) {
   int clickedRow = mousePosition.y / board.getSquareSize();
   int clickedCol = mousePosition.x / board.getSquareSize();

    if (isValidSquare(clickedRow, clickedCol)) {
        Square& clickedSquare = board.getSquare(clickedRow, clickedCol); // case actuel cliqué


        if (((currentPlayerIndex == 0 && clickedSquare.getPawn() != nullptr && clickedSquare.getPawn()->getColor() == PawnColor::WHITE) ||
            (currentPlayerIndex == 1 && clickedSquare.getPawn() != nullptr && clickedSquare.getPawn()->getColor() == PawnColor::BLACK))
            && clickedSquare.getCurrentColor() == SquareColor::CYAN) {
        
            board.resetSquareColorsDames();

            clickedSquare.setSelected(true);
            clickedSquare.setCurrentColor(SquareColor::CYAN); // Cette couleur représente la selection actuel du joueur

            pawnSelected = true;
            
            if (areJumpsPossibleForPlayer(currentPlayerIndex)) { // on priorise d'abord si il y a des sauts possible (pour pion ou dame)

                if (clickedSquare.getPawn()->getHasCrown()) { // on priorise les sauts de la dame au sauts des pions
                    highlightPossibleJumpsForDame(clickedSquare); // on colorie en vert les cases de saut pour la dame
                } else {
                    highlightPossibleJumps(clickedSquare); // on colorie en vert les cases de saut pour le pion
                }
            } else {
                if(clickedSquare.getPawn()->getHasCrown()) {       // meme choses pour les mouvements
                    highlightPossibleMovesForDame(clickedSquare);
                } else {
                    highlightPossibleMoves(clickedSquare);
                }
                
                
            }

        }
        else {
            if(clickedSquare.getCurrentColor() != SquareColor::GREEN) { // si on clique ailleur que sur une case verte
                board.resetSquareColorsDames();        
                pawnSelected = false;
                highlightSelectablePawns(currentPlayerIndex); // on remet les choix de selection des pions/dame que le joueur peut choisir
            }
        } 
        
    } 
}

// Gestion de deplacement d'un pion ou d'une dame
void DamesGame::movePawn(const sf::Vector2i& mousePosition) {
    int clickedRow = mousePosition.y / board.getSquareSize();
    int clickedCol = mousePosition.x / board.getSquareSize();

    if (isValidSquare(clickedRow, clickedCol)) {
        Square& clickedSquare = board.getSquare(clickedRow, clickedCol);

        if (clickedSquare.getCurrentColor() == SquareColor::GREEN) {  // si on a cliquer sur une case alors on a decider de bouger un pion ou une dame
            int targetRow = clickedSquare.getRow();
            int targetCol = clickedSquare.getCol();

            // Sauvegarde la position initiale du pion (nous servira pour savoir si il y a eu un saut avant)
            int initialRow = board.getSelectedSquare().getRow();
            int initialCol = board.getSelectedSquare().getCol();

            moveSelectedPawnTo(targetRow, targetCol);

            PawnMoved = true;

            board.resetSquareColorsDames();

            clickedSquare.setSelected(true);
            clickedSquare.setCurrentColor(SquareColor::CYAN);

            // Vérifie s'il y a eu un saut pour un pion
            if (std::abs(initialRow - targetRow) == 2 && std::abs(initialCol - targetCol) == 2) {
                
                // Si oui on Réaffiche les surbrillances des sauts possibles pour anticiper les nouveaux sauts possibles pour un pion normal
                if (areJumpsPossibleForPlayer(currentPlayerIndex) && clickedSquare.getPawn()->getHasCrown() == false) {
                    highlightPossibleJumps(clickedSquare);
                }
            }

        }
    }
}

// Methode pour deplacer d'une case A à une case B un pion
void DamesGame::moveSelectedPawnTo(int targetRow, int targetCol) {
    Square& selectedSquare = board.getSelectedSquare();

    // Vérifie si une case est sélectionnée et si elle contient un pion blanc ou noir
    if (selectedSquare.getPawn() != nullptr && (selectedSquare.getPawn()->getColor() == PawnColor::WHITE
    || selectedSquare.getPawn()->getColor() == PawnColor::BLACK)) {
        int selectedRow = selectedSquare.getRow();
        int selectedCol = selectedSquare.getCol();

        // Vérifie les limites de l'indice pour éviter les débordements
        if (targetRow >= 0 && targetRow < board.getRows() && targetCol >= 0 && targetCol < board.getCols()) {
            // Calcule de la position de la case du milieu
            int middleRow = (selectedRow + targetRow) / 2;
            int middleCol = (selectedCol + targetCol) / 2;

            // Vérifie si un saut a été effectué par un pion
            if ((std::abs(selectedRow - targetRow) == 2 && std::abs(selectedCol - targetCol) == 2 
            && selectedSquare.getPawn()->getHasCrown() == false)
            || (areJumpsPossibleForDame(selectedSquare) && selectedSquare.getPawn()->getHasCrown())) {  
                       
                // Déplace le pion vers la case cible
                board.getSquare(targetRow, targetCol).placePawn(*selectedSquare.getPawn());
                board.getSquare(selectedRow, selectedCol).removePawn();

                // On met la case du milieu en bleu pour marquer que le pion a etait sauté
                board.getSquare(middleRow, middleCol).setCurrentColor(SquareColor::BLUE);
            } else {
                // On déplace le pion normalement sans saut
                board.getSquare(targetRow, targetCol).placePawn(*selectedSquare.getPawn());
                board.getSquare(selectedRow, selectedCol).removePawn();
            }

        }
    }
}

// Passage d'un pion en dame
void DamesGame::promoteToDame(int row, int col) {
    // Récupere le pion existant
    Square& currentSquare = board.getSquare(row, col);
    
    // Vérifie que la case contient un pion
    if (currentSquare.getPawn() != nullptr) {

        // Créer une dame de la même couleur
        Pawn promotedPawn(currentSquare.getPawn()->getColor());
        promotedPawn.setHasCrown(true);

        // On Affecte la nouvelle dame à la case
        currentSquare.placePawn(promotedPawn);
    }

}

// Verification des mouvements possible pour un pion normal
bool DamesGame::areMovesPossible(const Square& pawnSquare) const {
    int forwardRow = currentPlayerIndex == 0 ? pawnSquare.getRow() - 1 : pawnSquare.getRow() + 1;
    int pawnCol = pawnSquare.getCol();

    bool movesPossible = false;

    if (isValidSquare(forwardRow, pawnCol - 1)) {
        Square& diagonalLeft = board.getSquare(forwardRow, pawnCol - 1);
        if (diagonalLeft.getPawn() == nullptr) {
            movesPossible = true;
        }
    }

    if (isValidSquare(forwardRow, pawnCol + 1) && !movesPossible) {
        Square& diagonalRight = board.getSquare(forwardRow, pawnCol + 1);
        if (diagonalRight.getPawn() == nullptr) {
            movesPossible = true;
        }
    }

    return movesPossible;
}

// colorie les cases en vert des mouvements possible pour un pion normal
void DamesGame::highlightPossibleMoves(const Square& currentSquare) {
    int forwardRow = currentPlayerIndex == 0 ? currentSquare.getRow() - 1 : currentSquare.getRow() + 1;
    int pawnCol = currentSquare.getCol();

    if (isValidSquare(forwardRow, pawnCol - 1)) {
        Square& diagonalLeft = board.getSquare(forwardRow, pawnCol - 1);
        if (diagonalLeft.getPawn() == nullptr) {
            diagonalLeft.setCurrentColor(SquareColor::GREEN); // Surbrillance
        }
    }

    if (isValidSquare(forwardRow, pawnCol + 1)) {
        Square& diagonalRight = board.getSquare(forwardRow, pawnCol + 1);
        if (diagonalRight.getPawn() == nullptr) {
            diagonalRight.setCurrentColor(SquareColor::GREEN); // Surbrillance
        }
    }
}


// Verification des sauts possible pour un pion normal
bool DamesGame::areJumpsPossible(const Square& currentSquare) const {
    int row = currentSquare.getRow();
    int col = currentSquare.getCol();

    bool jumpsPossible = false;

    // Vérification pour les sauts en diagonale avant gauche et droite en fonction du joueur
    int forwardRow = currentPlayerIndex == 0 ? row - 2 : row + 2; 
    int forwardRowPre = currentPlayerIndex == 0 ? row - 1 : row + 1;

    // Vérification pour les sauts en diagonale avant
    if (isValidSquare(forwardRow, col - 2)) {
        Square& diagonalLeft = board.getSquare(forwardRow, col - 2);
        Square& middleLeft = board.getSquare(forwardRowPre, col - 1);
        if (middleLeft.getCurrentColor()  != SquareColor::BLUE && 
            diagonalLeft.getPawn() == nullptr && middleLeft.getPawn() != nullptr 
            && middleLeft.getPawn()->getColor() != currentSquare.getPawn()->getColor()) {
            jumpsPossible = true;
        }
    }

    if (isValidSquare(forwardRow, col + 2) && !jumpsPossible) {
        Square& diagonalRight = board.getSquare(forwardRow, col + 2);
        Square& middleRight = board.getSquare(forwardRowPre, col + 1);
        if (middleRight.getCurrentColor() != SquareColor::BLUE && 
            diagonalRight.getPawn() == nullptr && middleRight.getPawn() != nullptr 
            && middleRight.getPawn()->getColor() != currentSquare.getPawn()->getColor()) {
            jumpsPossible = true;
        }
    }
    
    // Vérification pour les sauts en diagonale arrière gauche et droite en fonction du joueur
    int backwardRow = currentPlayerIndex == 0 ? row + 2 : row - 2;
    int backwardRowPre = currentPlayerIndex == 0 ? row + 1 : row - 1;

    // Vérification pour les sauts en diagonale arrière
    if (isValidSquare(backwardRow, col - 2)) {
        Square& diagonalLeft = board.getSquare(backwardRow, col - 2);
        Square& middleLeft = board.getSquare(backwardRowPre, col - 1);
        if (middleLeft.getCurrentColor() != SquareColor::BLUE &&  
            diagonalLeft.getPawn() == nullptr && middleLeft.getPawn() != nullptr 
            && middleLeft.getPawn()->getColor() != currentSquare.getPawn()->getColor()) {
            jumpsPossible = true;
        }
    }

    if (isValidSquare(backwardRow, col + 2) && !jumpsPossible) {
        Square& diagonalRight = board.getSquare(backwardRow, col + 2);
        Square& middleRight = board.getSquare(backwardRowPre, col + 1);
        if (middleRight.getCurrentColor() != SquareColor::BLUE && 
            diagonalRight.getPawn() == nullptr && middleRight.getPawn() != nullptr 
            && middleRight.getPawn()->getColor() != currentSquare.getPawn()->getColor()) {
            jumpsPossible = true;
        }
    }
   
    return jumpsPossible;
}


// colorie les cases en vert des sauts possible pour un pion normal
void DamesGame::highlightPossibleJumps(const Square& currentSquare) {
    int row = currentSquare.getRow();
    int col = currentSquare.getCol();

    int forwardRow = currentPlayerIndex == 0 ? row - 2 : row + 2;
    int forwardRowPre = currentPlayerIndex == 0 ? row - 1 : row + 1;

    // Mise en surbrillance pour les sauts en diagonale avant
    if (isValidSquare(forwardRow, col - 2)) {
        Square& diagonalLeft = board.getSquare(forwardRow, col - 2);
        Square& middleLeft = board.getSquare(forwardRowPre, col - 1);
        if (middleLeft.getCurrentColor() != SquareColor::BLUE &&  
            diagonalLeft.getPawn() == nullptr && middleLeft.getPawn() != nullptr 
            && middleLeft.getPawn()->getColor() != currentSquare.getPawn()->getColor()) {
            diagonalLeft.setCurrentColor(SquareColor::GREEN); // Surbrillance
        }
    }

    if (isValidSquare(forwardRow, col + 2)) {
        Square& diagonalRight = board.getSquare(forwardRow, col + 2);
        Square& middleRight = board.getSquare(forwardRowPre, col + 1);
        if (middleRight.getCurrentColor() != SquareColor::BLUE &&  
            diagonalRight.getPawn() == nullptr && middleRight.getPawn() != nullptr 
            && middleRight.getPawn()->getColor() != currentSquare.getPawn()->getColor()) {
            diagonalRight.setCurrentColor(SquareColor::GREEN); // Surbrillance
        }
    }

    int backwardRow = currentPlayerIndex == 0 ? row + 2 : row - 2;
    int backwardRowPre = currentPlayerIndex == 0 ? row + 1 : row - 1;

    // Mise en surbrillance pour les sauts en diagonale arrière
    if (isValidSquare(backwardRow, col - 2)) {
        Square& diagonalLeft = board.getSquare(backwardRow, col - 2);
        Square& middleLeft = board.getSquare(backwardRowPre, col - 1);
        if (middleLeft.getCurrentColor() != SquareColor::BLUE &&  
            diagonalLeft.getPawn() == nullptr && middleLeft.getPawn() != nullptr 
            && middleLeft.getPawn()->getColor() != currentSquare.getPawn()->getColor()) {
            diagonalLeft.setCurrentColor(SquareColor::GREEN); // Surbrillance
        }
    }

    if (isValidSquare(backwardRow, col + 2)) {
        Square& diagonalRight = board.getSquare(backwardRow, col + 2);
        Square& middleRight = board.getSquare(backwardRowPre, col + 1);
        if (middleRight.getCurrentColor() != SquareColor::BLUE && 
            diagonalRight.getPawn() == nullptr && middleRight.getPawn() != nullptr 
            && middleRight.getPawn()->getColor() != currentSquare.getPawn()->getColor()) {
            diagonalRight.setCurrentColor(SquareColor::GREEN); // Surbrillance
        }
    }

}


// On verifie si des sauts sont possible pour un joueur donné
bool DamesGame::areJumpsPossibleForPlayer(int playerIndex) const {

    // Parcourez toutes les cases pour le joueur spécifié
    for (int row = 0; row < board.getRows(); ++row) {
        for (int col = 0; col < board.getCols(); ++col) {
            Square& currentSquare = board.getSquare(row, col);

            if ((playerIndex == 0 && currentSquare.getPawn() != nullptr && currentSquare.getPawn()->getColor() == PawnColor::WHITE) ||
                (playerIndex == 1 && currentSquare.getPawn() != nullptr && currentSquare.getPawn()->getColor() == PawnColor::BLACK)) {
                
                // Vérifiez si des prises sont possibles pour un pion normal
                if (areJumpsPossible(currentSquare)) {
                    return true;
                }
            }
        }
    }

    return false;
}

// On guide les joueurs a choisir quel pion selectionné (dame ou pion)
void DamesGame::highlightSelectablePawns(int playerIndex) {
    // Parcoure toutes les cases pour le joueur spécifié
    for (int row = 0; row < board.getRows(); ++row) {
        for (int col = 0; col < board.getCols(); ++col) {
            Square& currentSquare = board.getSquare(row, col);

            if ((playerIndex == 0 && currentSquare.getPawn() != nullptr && currentSquare.getPawn()->getColor() == PawnColor::WHITE) ||
                (playerIndex == 1 && currentSquare.getPawn() != nullptr && currentSquare.getPawn()->getColor() == PawnColor::BLACK)) {

                 // Vérifie si des prises sont possibles pour ce joueur
                if (areJumpsPossibleForPlayer(currentPlayerIndex)) {
                    // Colorie uniquement les pions/dames qui peuvent effectuer des sauts pour un pion ou une dame
                    if (areJumpsPossible(currentSquare) && currentSquare.getPawn()->getHasCrown() == false) {
                        currentSquare.setCurrentColor(SquareColor::CYAN);
                    }
                    if (currentSquare.getPawn()->getHasCrown()) {
                        currentSquare.setCurrentColor(SquareColor::CYAN);
                    }
                } else {
                    // Colorie uniquement les pions/dames qui peuvent se déplacer pour un pion ou une dame
                    if (currentSquare.getPawn()->getHasCrown()) {
                        currentSquare.setCurrentColor(SquareColor::CYAN);
                    } else if (areMovesPossible(currentSquare)) {
                        currentSquare.setCurrentColor(SquareColor::CYAN);
                    }
                }
            }
        }
    }

}

// On colorie en vert les mouvements possibles pour la dame
void DamesGame::highlightPossibleMovesForDame(const Square& currentSquare) {

    // Vérifie les mouvements possibles vers le haut et vers le bas
    checkDiagonalMoves(currentSquare, -1, -1);
    checkDiagonalMoves(currentSquare, -1, 1);
    checkDiagonalMoves(currentSquare, 1, -1);
    checkDiagonalMoves(currentSquare, 1, 1);
}

void DamesGame::checkDiagonalMoves(const Square& currentSquare, int rowIncrement, int colIncrement) {
    int pawnRow = currentSquare.getRow();
    int pawnCol = currentSquare.getCol();

    int targetRow = pawnRow + rowIncrement;
    int targetCol = pawnCol + colIncrement;

    bool canMove = true;

    while (isValidSquare(targetRow, targetCol)) {
        Square& targetSquare = board.getSquare(targetRow, targetCol);

        // Vérifie si la case cible est occupée par une pièce adverse
        if (targetSquare.getPawn() != nullptr && targetSquare.getPawn()->getColor() != currentSquare.getPawn()->getColor()) {
            canMove = false;
            break;
        }

        // Déplace la position cible vers le haut ou vers le bas
        targetRow += rowIncrement;
        targetCol += colIncrement;
    }

    if (canMove) {
        // Marque les cases vides sur la diagonale comme déplaçables
        targetRow = pawnRow + rowIncrement;
        targetCol = pawnCol + colIncrement;

        while (isValidSquare(targetRow, targetCol)) {
            Square& targetSquare = board.getSquare(targetRow, targetCol);

            if (targetSquare.getPawn() == nullptr) {
                targetSquare.setCurrentColor(SquareColor::GREEN); // Surbrillance
            } else {
                break;  // Arrête si une pièce est rencontrée (ce cas ne devrait pas se produire ici)
            }

            targetRow += rowIncrement;
            targetCol += colIncrement;
        }
    }
}

// On verifie si des sauts sont possibles pour la dame
bool DamesGame::areJumpsPossibleForDame(const Square& currentSquare) const {
    // Vérification pour les sauts en diagonale
    return isJumpPossibleForDame(currentSquare, -1, -1) ||
           isJumpPossibleForDame(currentSquare, -1, 1) ||
           isJumpPossibleForDame(currentSquare, 1, -1) ||
           isJumpPossibleForDame(currentSquare, 1, 1);
}

bool DamesGame::isJumpPossibleForDame(const Square& currentSquare, int directionRow, int directionCol) const {
    int row = currentSquare.getRow();
    int col = currentSquare.getCol();

    // Coordonnées des cases suivantes dans la direction spécifiée
    int nextRow = row + directionRow;
    int nextCol = col + directionCol;
    int targetRow = row + 2 * directionRow;
    int targetCol = col + 2 * directionCol;

    // Vérification des limites de l'indice
    if (!isValidSquare(nextRow, nextCol) || !isValidSquare(targetRow, targetCol)) {
        return false;
    }

    Square& nextSquare = board.getSquare(nextRow, nextCol);
    Square& targetSquare = board.getSquare(targetRow, targetCol);

    // Vérification de la présence d'un seul pion adverse suivi d'une case vide
    return (nextSquare.getPawn() != nullptr && nextSquare.getPawn()->getColor() != currentSquare.getPawn()->getColor() && targetSquare.getPawn() == nullptr);
}


void DamesGame::highlightPossibleJumpsForDame(const Square& currentSquare) {
    int row = currentSquare.getRow();
    int col = currentSquare.getCol();

    // Mise en surbrillance pour les sauts en diagonale
    if (isJumpPossibleForDame(currentSquare, -1, -1)) {
        board.getSquare(row - 2, col - 2).setCurrentColor(SquareColor::GREEN); // Surbrillance
    }

    if (isJumpPossibleForDame(currentSquare, -1, 1)) {
        board.getSquare(row - 2, col + 2).setCurrentColor(SquareColor::GREEN); // Surbrillance
    }

    if (isJumpPossibleForDame(currentSquare, 1, -1)) {
        board.getSquare(row + 2, col - 2).setCurrentColor(SquareColor::GREEN); // Surbrillance
    }

    if (isJumpPossibleForDame(currentSquare, 1, 1)) {
        board.getSquare(row + 2, col + 2).setCurrentColor(SquareColor::GREEN); // Surbrillance
    }
}

// les limites du plateau
bool DamesGame::isValidSquare(int row, int col) const {
    return row >= 0 && row < board.getRows() && col >= 0 && col < board.getCols();
}

// Gestion de la souris
void DamesGame::handleMouseClick(const sf::Vector2i& mousePosition) {

    if(!PawnMoved){
        selectPawn(mousePosition);
    }
    movePawn(mousePosition);
}

// Boucle de la logique principal du jeu à chaque action qu'on fait, on doit cliquer sur le bouton "confirmer"
void DamesGame::mainGameLoop(sf::RenderWindow& window) {

    if(!pawnSelected && !PawnMoved) {
        highlightSelectablePawns(currentPlayerIndex);
    }

    // Vérifie si le bouton "Confirmer" a été cliqué et qu'un pion a été sélectionné
    if (pawnSelected && isConfirmButtonClicked(window)) {

        // Récupére les pions sautés
        vector<Pawn> jumpedPawns = board.getJumpedPawns();

        // Sauvegarde le pion capturé par le joueur
        players[currentPlayerIndex].capturePawns(jumpedPawns);

        // Vérifie si le pion peut être promu en dame
        Square& selectedSquare = board.getSelectedSquare();
        int targetRow = selectedSquare.getRow();

        if (((currentPlayerIndex == 1 && targetRow == board.getRows() - 1) ||(currentPlayerIndex == 0 && targetRow == 0))) {
            promoteToDame(targetRow, selectedSquare.getCol());
        }

        // Réinitialisation du plateau les pions sautés (cases bleues)
        board.removeJumpedPawns();
        board.resetSquareColorsDames();

        // Passe au joueur suivant
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();

        // Réinitialisation le marquage de sélection et de déplacement du pion 
        pawnSelected = false;
        PawnMoved = false;
    }

    // Vérifie la fin du jeu ici, car le joueur a effectué une action et a confirmé
    if (isGameOver()) {
        
        gameEnded = true;
    }
}



// On verifie si un des 2 joueurs possede encore des pions/dames
bool DamesGame::isPlayerOutOfPawns(int playerIndex) const {
    for (int row = 0; row < board.getRows(); ++row) {
        for (int col = 0; col < board.getCols(); ++col) {
            const Square& currentSquare = board.getSquare(row, col);

            if ((playerIndex == 0 && currentSquare.getPawn() != nullptr && currentSquare.getPawn()->getColor() == PawnColor::WHITE) ||
                (playerIndex == 1 && currentSquare.getPawn() != nullptr && currentSquare.getPawn()->getColor() == PawnColor::BLACK)) {
                return false;  // Oui donc on continue de jouer
            }
        }
    }

    return true;  // Non donc la partie se termine
}

// verification de fin de partie
bool DamesGame::isGameOver() const{
    
    if (isPlayerOutOfPawns(currentPlayerIndex)) {

        // Retire tous les pions restants sur le plateau
        board.removeRemainingPawns();

        board.resetSquareColorsDames();

        return true;
    }

    return false;
}


//Bouton retour en sfml
void DamesGame::drawBackButton(sf::RenderWindow& window) const {

    // Initialise le bouton "Retour"
    backButton.setSize(sf::Vector2f(100, 50));
    backButton.setFillColor(sf::Color(255, 69, 0));
    backButton.setPosition(window.getSize().x - backButton.getSize().x - 20, 20);

    // Dessine le bouton "Retour"
    window.draw(backButton);

    sf::Font font;
    if (font.loadFromFile("arial.ttf")) {
        // Ajoute du texte au bouton "Retour"
        sf::Text backButtonText("Retour", font, 20);
        backButtonText.setFillColor(sf::Color::White);
        backButtonText.setPosition(backButton.getPosition().x + backButton.getSize().x / 2.0f, backButton.getPosition().y + backButton.getSize().y / 2.0f);
        backButtonText.setOrigin(backButtonText.getLocalBounds().left + backButtonText.getLocalBounds().width / 2.0f, backButtonText.getLocalBounds().top + backButtonText.getLocalBounds().height / 2.0f);
        
        // Dessine le texte sur le bouton
        window.draw(backButtonText);
    }
    

}

// Bouton confirmer
void DamesGame::drawConfirmButton(sf::RenderWindow& window) const{
    
    if (PawnMoved && !board.remainingMouvments()) {
        // On recupere la taille de la fenetre
        sf::Vector2u windowSize = window.getSize();

        // Initialise le bouton "Confirmer"
        confirmButton.setSize(sf::Vector2f(100, 50)); // Taille du bouton

        // on le met en bas a droite de la fenetre
        confirmButton.setPosition(windowSize.x - confirmButton.getSize().x - 20, windowSize.y - confirmButton.getSize().y - 20);

        // couleur bleu du bouton
        confirmButton.setFillColor(sf::Color(30, 144, 255));

        // dessin juste le bouton sans texte
        window.draw(confirmButton);

        // ajoute du texte
        sf::Font font;
        if (font.loadFromFile("arial.ttf")) {
            sf::Text buttonText("Confirmer", font, 20); // la taille du texte
            buttonText.setFillColor(sf::Color::White);

            // On centre le texte à l'intérieur du bouton
            sf::FloatRect textBounds = buttonText.getLocalBounds();
            buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
            buttonText.setPosition(confirmButton.getPosition().x + confirmButton.getSize().x / 2.0f, confirmButton.getPosition().y + confirmButton.getSize().y / 2.0f);

            // On dessine le texte a linterieur du bouton
            window.draw(buttonText);
        }
    }
  
    
}

// on verifie si le bouton confirmer a etait cliqué
bool DamesGame::isConfirmButtonClicked(const sf::RenderWindow& window) const {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    return confirmButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))
        && sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

// Ici seront afficher les informations sur le deroulement du jeu et des joueurs
void DamesGame::drawGameInfo(sf::RenderWindow& window) const {
    sf::Font font;
    if (font.loadFromFile("arial.ttf")) {

        // Affiche les informations des joueurs
        float infoYPosition = 100;

        if(!gameEnded){
            // Affiche le tour du joueur actuel
            sf::Text playerTurnText("Au tour de " + players[currentPlayerIndex].getName(), font, 20);
            playerTurnText.setFillColor(sf::Color::White);
            playerTurnText.setPosition(board.getRows() * board.getSquareSize() + 20, infoYPosition);
            window.draw(playerTurnText);

            // Ajoute un espace
            infoYPosition += 60;

        }

        // Boucle sur les joueurs
        for (const auto& player : players) {

            // Affiche les informations des joueurs seulement si le tableau n'est pas vide
            if (!player.getPawnsCaptured().empty()) {

                // Affiche le nom du joueur
                sf::Text playerInfo(player.getName(), font, 20);
                playerInfo.setFillColor(sf::Color::White);
                playerInfo.setPosition(board.getRows() * board.getSquareSize() + 20, infoYPosition);
                window.draw(playerInfo);

                // Ajoute un espace
                infoYPosition += 40;

                // Affiche l'en-tête des pions capturés
                sf::Text headerText("Les captures :", font, 20);
                headerText.setFillColor(sf::Color::White);
                headerText.setPosition(board.getRows() * board.getSquareSize() + 20, infoYPosition);
                window.draw(headerText);

                // Affiche les pions capturés 
                // Variables pour stocker le nombre de pions et de dames capturés
                int countPawns = 0;
                int countDames = 0;

                // Parcourt du tableau des pions capturés
                for (const auto& capturedPawn : player.getPawnsCaptured()) {
                    if (capturedPawn.getHasCrown()) {
                        // Si c'est une dame, on incrémente le compteur des dames
                        countDames++;
                    } else {
                        // Sinon, on incrémente le compteur des pions
                        countPawns++;
                    }
                }

                // On Affiche le score à droite de "Pions captures"
                sf::Text scoreText("Score :", font, 20);
                scoreText.setFillColor(sf::Color::White);
                scoreText.setPosition(board.getRows() * board.getSquareSize() + 200, infoYPosition);
                window.draw(scoreText);

                // Ajoute un espace
                infoYPosition += 30;
                

                // Affiche la valeur du score
                int total = countPawns + countDames;
                sf::Text scoreValueText(std::to_string(total), font, 20);
                players[currentPlayerIndex].setScore(total);
                scoreValueText.setFillColor(sf::Color::White);
                scoreValueText.setPosition(board.getRows() * board.getSquareSize() + 200, infoYPosition);
                window.draw(scoreValueText);

                // Affiche les pions capturés
                std::string pawnsInfo = "Pion x" + std::to_string(countPawns);
                sf::Text pawnsText(pawnsInfo, font, 20);
                pawnsText.setFillColor(sf::Color::White);
                pawnsText.setPosition(board.getRows() * board.getSquareSize() + 20, infoYPosition);
                window.draw(pawnsText);

                // Ajoute un espace
                infoYPosition += 30;

                // Affiche les dames capturées
                std::string damesInfo = "Dame x" + std::to_string(countDames);
                sf::Text damesText(damesInfo, font, 20);
                damesText.setFillColor(sf::Color::White);
                damesText.setPosition(board.getRows() * board.getSquareSize() + 20, infoYPosition);
                window.draw(damesText);

                // Ajoute un espace
                infoYPosition += 30;

            }

            // Ajoute un espace avant le prochain joueur
            infoYPosition += 40;
        }

        // Vérifie si le jeu est terminé
        if (gameEnded) {
            // Trouve le joueur avec le score le plus élevé entre joueur 1 et joueur 2
            const Player& player1 = players[0];
            const Player& player2 = players[1];

            const Player* winner = (player1.calculateScore() > player2.calculateScore()) ? &player1 : &player2;

            // Affiche le vainqueur
            sf::Text winnerText("Le vainqueur est " + winner->getName(), font, 20);
            winnerText.setFillColor(sf::Color::Green);
            winnerText.setPosition(board.getRows() * board.getSquareSize() + 20, infoYPosition);
            window.draw(winnerText);

            // Dessine le bouton "Rejouer"
            drawRestartButton(window);
        }
    }
}


// bouton rejouer
void DamesGame::drawRestartButton(sf::RenderWindow& window) const {
    // Positionne le bouton "Rejouer" à gauche du bouton "Confirmer"
    restartButton.setSize(sf::Vector2f(100, 50));
    restartButton.setPosition(window.getSize().x - restartButton.getSize().x - 20, window.getSize().y - restartButton.getSize().y - 20);
    restartButton.setFillColor(sf::Color(30, 144, 255));
          
    // Dessine le bouton "Rejouer"
    window.draw(restartButton);

    // Ajoute du texte au bouton
    sf::Font font;
    if (font.loadFromFile("arial.ttf")) {
        sf::Text buttonText("Rejouer", font, 20);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(restartButton.getPosition().x + restartButton.getSize().x / 2.0f, restartButton.getPosition().y + restartButton.getSize().y / 2.0f);
        buttonText.setOrigin(buttonText.getLocalBounds().left + buttonText.getLocalBounds().width / 2.0f, buttonText.getLocalBounds().top + buttonText.getLocalBounds().height / 2.0f);
        window.draw(buttonText);
    }
    
}

// Mise a jour de la fenetre d'affichage au file du jeu
void DamesGame::updateDisplay(sf::RenderWindow& window) {
    window.clear();

    // Dessine le plateau
    board.draw(window);

    // Dessine le bouton "Retour"
    drawBackButton(window);

    // Dessine le bouton "Confirmer"
    drawConfirmButton(window);
    
    // Dessine les informations du jeu
    drawGameInfo(window);

    window.display();
}


