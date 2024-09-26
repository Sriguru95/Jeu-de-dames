#include "ButinGame.hpp"
#include <iostream>

ButinGame::ButinGame(const std::vector<std::string>& playerNames)
    : board{8, 8, sf::Vector2u(1200, 800)},
      players{},
      currentPlayerIndex{0},
      pawnSelected{false},
      yellowPawnMoved{false},
      gameEnded{false},
      restartRequested{false},
      returnToMenuRequested{false} {

    // Initialisation des joueurs
    for (const auto& playerName : playerNames) {
        players.push_back(Player(playerName));
    }
    
}

ButinGame::~ButinGame() {
    // Éventuelles opérations de nettoyage
}


void ButinGame::run(sf::RenderWindow& window) {

    window.setTitle("Jeu de Butin");

    createBoard();

    // Boucle principale du jeu
    while (window.isOpen() && !returnToMenuRequested) {
        handleWindowEvents(window);
        updateDisplay(window);
    }
}

bool ButinGame::hasReturnToMenuRequested() const {
        return returnToMenuRequested;
}

void ButinGame::resetGame() {
    // Réinitialise toutes les variables du jeu ici (comme vous l'avez fait dans le constructeur)
    currentPlayerIndex = 0;
    pawnSelected = false;
    yellowPawnMoved = false;
    gameEnded = false;
    restartRequested = false;

    // Réinitialise les joueurs si nécessaire
    for (auto& player : players) {
        player.reset(); 
    }

    createBoard();
}

void ButinGame::createBoard() {
    // Initialise le plateau de jeu
    board.setupButinGame();
}

void ButinGame::handleWindowEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                
                // Gérer le clic de souris sur le plateau
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                handleMouseClick(mousePosition);

                // Gérer le clic du bouton "Rejouer"
                if (gameEnded && restartButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))
                    && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    restartRequested = true;
                }
                
                // Gérer le clic du bouton "Retour"
                if (backButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))
                    && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    returnToMenuRequested = true;
                    window.setTitle("Jeux de plateau");
                }
           
            }
        }
    }

    prepareGame(window);

    mainGameLoop(window);

    // Si un redémarrage est demandé, on réinitialise le jeu
    if (restartRequested) {
        resetGame();
    }
}

void ButinGame::selectPawn(const sf::Vector2i& mousePosition) {
    int clickedRow = mousePosition.y / board.getSquareSize();
    int clickedCol = mousePosition.x / board.getSquareSize();

    if (isValidSquare(clickedRow, clickedCol)) {
        Square& clickedSquare = board.getSquare(clickedRow, clickedCol);

        if (clickedSquare.getPawn() != nullptr && clickedSquare.getPawn()->getColor() == PawnColor::YELLOW) {

            // Vérifie si un autre pion jaune avait déjà été sélectionné
            if (pawnSelected) {
                // Si oui, réinitialiser les variables avant de sélectionner le nouveau pion
                board.resetSquareColors();
                pawnSelected = false;
                yellowPawnMoved = false;
            }
            
            // condition pour vérifier si un pion jaune est déjà sélectionné et deplacé
            if (!pawnSelected && !yellowPawnMoved) {
                board.resetSquareColors();

                clickedSquare.setSelected(true);
                clickedSquare.setCurrentColor(SquareColor::CYAN);

                if (players[currentPlayerIndex].getFirstTime()) {
                    board.highlightPossibleJumps();
                }
            }
        }
    }
}


void ButinGame::movePawn(const sf::Vector2i& mousePosition) {
    int clickedRow = mousePosition.y / board.getSquareSize();
    int clickedCol = mousePosition.x / board.getSquareSize();

    if (isValidSquare(clickedRow, clickedCol)) {
        Square& clickedSquare = board.getSquare(clickedRow, clickedCol);

        if (clickedSquare.getCurrentColor() == SquareColor::GREEN) {
            int targetRow = clickedSquare.getRow();
            int targetCol = clickedSquare.getCol();

            moveSelectedPawnTo(targetRow, targetCol);

            yellowPawnMoved = true;

            board.resetSquareColors();

            clickedSquare.setSelected(true);
            clickedSquare.setCurrentColor(SquareColor::CYAN);

            board.highlightPossibleJumps();
        }
    }
}


void ButinGame::moveSelectedPawnTo(int targetRow, int targetCol) {
    Square& selectedSquare = board.getSelectedSquare();

    // Vérifie si une case est sélectionnée et si elle contient un pion jaune
    if (selectedSquare.getPawn() != nullptr && selectedSquare.getPawn()->getColor() == PawnColor::YELLOW ) {
        int selectedRow = selectedSquare.getRow();
        int selectedCol = selectedSquare.getCol();

        // Enregistre la position actuelle du pion jaune
        sf::Vector2i currentPosition(selectedRow, selectedCol);

        // Vérifie les limites de l'indice pour éviter les débordements
        if (targetRow >= 0 && targetRow < board.getRows() && targetCol >= 0 && targetCol < board.getCols()) {
            // Déplace le pion vers la case cible
            board.getSquare(targetRow, targetCol).placePawn(*selectedSquare.getPawn()); 
            board.getSquare(selectedRow, selectedCol).removePawn(); 

            // On recupere la position du pion sauté
            int jumpedRow = (currentPosition.x + targetRow) / 2;
            int jumpedCol = (currentPosition.y + targetCol) / 2;

            // Colorie la case du pion sauté en bleu
            board.setSquareColor(jumpedRow, jumpedCol, SquareColor::BLUE);
        }
    }
}

bool ButinGame::isValidSquare(int row, int col) const {
    return row >= 0 && row < board.getRows() && col >= 0 && col < board.getCols();
}

void ButinGame::handleMouseClick(const sf::Vector2i& mousePosition) {
    
    selectPawn(mousePosition);
    movePawn(mousePosition);

}

void ButinGame::prepareGame(sf::RenderWindow& window) {

    // Récupére la case sélectionnée
    Square& selectedSquare = board.getSelectedSquare();

    // Vérifie si la case est valide et contient un pion jaune
    if (selectedSquare.getPawn() != nullptr && selectedSquare.getPawn()->getColor() == PawnColor::YELLOW) {

        // Vérifie si le joueur pour la 1ere fois n'a pas encore retiré de pion jaune pendant la préparation 
        if (!players[currentPlayerIndex].getFirstTime()) {

            // Marquer qu'un pion a été sélectionné
            pawnSelected = true;
        }
    }

    // Vérifie si le bouton "Confirmer" a été cliqué et qu'un pion a été sélectionné
    if (pawnSelected && isConfirmButtonClicked(window)) {
        board.resetSquareColors();

        int selectedRow = selectedSquare.getRow();
        int selectedCol = selectedSquare.getCol();

        // Sauvegarder le pion capturé par le joueur
        players[currentPlayerIndex].capturePawn(*selectedSquare.getPawn());

        // On retire le pion du plateau
        board.removePawnAt(selectedRow, selectedCol);

        // Marque qu'un pion jaune a été retiré pour la 1ere fois
        players[currentPlayerIndex].setFirstTime(true);

        // Passer au joueur suivant
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();

        // Réinitialiser le marquage de sélection de pion et du deplacement du pion jaune
        pawnSelected = false;
        yellowPawnMoved = false;
    }
}


// Logique principale du jeu
void ButinGame::mainGameLoop(sf::RenderWindow& window) {


    // Vérifie si le joueur a déplacé un pion jaune et si le bouton "Confirmer" a été cliqué
    if (yellowPawnMoved && isConfirmButtonClicked(window)) {

        // Récupére les pions sautés
        vector<Pawn> jumpedPawns = board.getJumpedPawns();

        // Sauvegarde le pion capturé par le joueur
        players[currentPlayerIndex].capturePawns(jumpedPawns);

        // Retire du plateau les pions sautés (cases bleues)
        board.removeJumpedPawns();

        board.resetSquareColors();

        // Passe au joueur suivant
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();

        // Réinitialise les variables de suivi
        pawnSelected = false;
        yellowPawnMoved = false;

        // Vérifie la fin du jeu ici, car le joueur a effectué une action et a confirmé
        if (isGameOver()) {
            // On affiche le vainqueur
            int winningPlayerIndex = getWinningPlayerIndex();
            std::cout << "Le vainqueur est " << players[winningPlayerIndex].getName() << std::endl;
            gameEnded = true;
        }

    } 
}

// verifie la fin de partie
bool ButinGame::isGameOver() const{
    // Après que les joueurs ont joué pour la première fois, vérifier si les pions jaunes peuvent encore sauter
    if (players[currentPlayerIndex].getFirstTime() && !board.canYellowPawnsJump()) {
        std::cout << "La partie est terminee. Aucun deplacement possible pour les pions jaunes." << std::endl;

        // On applique un malus sur le score du dernier joueur qui a joué
        int malus = board.calculateRemainingPawnsPoints();
        int lastPlayerIndex = (currentPlayerIndex - 1 + players.size()) % players.size();
        players[lastPlayerIndex].applyScoreMalus(malus);

        // On retire tous les pions restants sur le plateau
        board.removeRemainingPawns();

        board.resetSquareColors();

        return true;
    }

    return false;
}

// determine le vainqueur
int ButinGame::getWinningPlayerIndex() const {
    int maxScore = -1;
    int winningPlayerIndex = -1;
    for (size_t i = 0; i < players.size(); ++i) {
        int playerScore = players[i].calculateScore();
        if (playerScore > maxScore) {
            maxScore = playerScore;
            winningPlayerIndex = i;
        }
    }
    return winningPlayerIndex;
}


//Boutton retour
void ButinGame::drawBackButton(sf::RenderWindow& window) const {

   
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
void ButinGame::drawConfirmButton(sf::RenderWindow& window) const{
  
    // Dessine le bouton seulement si un pion est sélectionné ou a été déplacé
    if (pawnSelected || yellowPawnMoved) {
        
        sf::Vector2u windowSize = window.getSize();

        confirmButton.setSize(sf::Vector2f(100, 50)); // Taille du bouton

        confirmButton.setPosition(windowSize.x - confirmButton.getSize().x - 20, windowSize.y - confirmButton.getSize().y - 20);

        confirmButton.setFillColor(sf::Color(30, 144, 255));

        window.draw(confirmButton);

        sf::Font font;
        if (font.loadFromFile("arial.ttf")) {
            sf::Text buttonText("Confirmer", font, 20); 
            buttonText.setFillColor(sf::Color::White);
           
            sf::FloatRect textBounds = buttonText.getLocalBounds();
            buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
            buttonText.setPosition(confirmButton.getPosition().x + confirmButton.getSize().x / 2.0f, confirmButton.getPosition().y + confirmButton.getSize().y / 2.0f);

            window.draw(buttonText);
        }
    }
}


bool ButinGame::isConfirmButtonClicked(const sf::RenderWindow& window) const {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    return confirmButton.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))
        && sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

// Panel d'information sur le deroulement du jeu et des joueurs
void ButinGame::drawGameInfo(sf::RenderWindow& window) const {
    sf::Font font;
    if (font.loadFromFile("arial.ttf")) {

        // Affiche les informations des joueurs
        float infoYPosition = 100; 

        if(!gameEnded){
            // Affiche le tour du joueur
            sf::Text playerTurnText("Au tour de " + players[currentPlayerIndex].getName(), font, 20);
            playerTurnText.setFillColor(sf::Color::White);
            playerTurnText.setPosition(board.getRows() * board.getSquareSize() + 20, infoYPosition);
            window.draw(playerTurnText);

            // Ajoute un espace
            infoYPosition += 40;

            // Affiche l'instruction en fonction de la première fois ou non
            sf::Text instructionText(players[currentPlayerIndex].getFirstTime() ? "Choisissez et deplacez un pion jaune" :  "Retirez un pion jaune", font, 20);
            instructionText.setFillColor(sf::Color::White);
            instructionText.setPosition(board.getRows() * board.getSquareSize() + 20, infoYPosition);  
            window.draw(instructionText);
        }
        
        // Ajoute un espace
        infoYPosition += 60;

        for (const auto& player : players) {

            // Affiche les pions capturés et les scores seulement si le tableau n'est pas vide
            if (!player.getPawnsCaptured().empty()) {

                // Affiche le nom du joueur
                sf::Text playerInfo(player.getName(), font, 20);
                playerInfo.setFillColor(sf::Color::White);
                playerInfo.setPosition(board.getRows() * board.getSquareSize() + 20, infoYPosition);
                window.draw(playerInfo);

                // Ajoute un espace
                infoYPosition += 40;

                // Affiche l'en-tête des pions capturés
                sf::Text headerText("Pions captures :", font, 20);
                headerText.setFillColor(sf::Color::White);
                headerText.setPosition(board.getRows() * board.getSquareSize() + 20, infoYPosition);
                window.draw(headerText);

                // Affiche le score à droite de "Pions captures"
                sf::Text scoreText("Score :", font, 20);
                scoreText.setFillColor(sf::Color::White);
                scoreText.setPosition(board.getRows() * board.getSquareSize() + 200, infoYPosition);
                window.draw(scoreText);

                // Ajoute un espace
                infoYPosition += 30;

                // Affiche la vauleur du score
                sf::Text scoreValueText(std::to_string(player.calculateScore()), font, 20);
                scoreValueText.setFillColor(sf::Color::White);
                scoreValueText.setPosition(board.getRows() * board.getSquareSize() + 200, infoYPosition);
                window.draw(scoreValueText);

                // Affiche les pions capturés 
                for (PawnColor color : {PawnColor::YELLOW, PawnColor::RED, PawnColor::BLACK}) {
                    int count = player.getCountCapturedPawns(color);
                    Pawn samplePawn(color); // Create a sample pawn with the specified color
                    std::string pawnInfo = samplePawn.getColorString() + " x" + std::to_string(count);
                    sf::Text pawnText(pawnInfo, font, 20);
                    pawnText.setFillColor(sf::Color::White);
                    pawnText.setPosition(board.getRows() * board.getSquareSize() + 20, infoYPosition);
                    window.draw(pawnText);

                    // Ajoute un espace
                    infoYPosition += 30;
                }

                // Ajuste la position Y pour le prochain joueur
                infoYPosition += 40;
            }
        }

        // Vérifie si le jeu est terminé
        if (gameEnded) {
            // Affiche le vainqueur
            int winningPlayerIndex = getWinningPlayerIndex();
            sf::Text winnerText("Le vainqueur est " + players[winningPlayerIndex].getName(), font, 24);
            winnerText.setFillColor(sf::Color::Green);
            winnerText.setPosition(board.getRows() * board.getSquareSize() + 20, infoYPosition);
            window.draw(winnerText);

            // Dessine le bouton "Rejouer"
            drawRestartButton(window);
        }

    }
}

// Bouton rejouer
void ButinGame::drawRestartButton(sf::RenderWindow& window) const {
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



void ButinGame::updateDisplay(sf::RenderWindow& window) {
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


