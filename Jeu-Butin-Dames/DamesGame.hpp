#include "AbstractGame.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class DamesGame : public AbstractGame {
public:
    DamesGame(const std::vector<std::string>& playerNames);

    void run(sf::RenderWindow& window) override;

    bool hasReturnToMenuRequested() const override;

protected:
    void handleWindowEvents(sf::RenderWindow& window) override;
    void updateDisplay(sf::RenderWindow& window) override;

private:
    mutable Board board;
    mutable std::vector<Player> players;
    int currentPlayerIndex;
    bool pawnSelected;
    bool PawnMoved;
    mutable sf::RectangleShape confirmButton;
    bool gameEnded;
    bool restartRequested;
    bool returnToMenuRequested;
    mutable sf::RectangleShape restartButton;
    mutable sf::RectangleShape backButton;
    

    void resetGame();

    void createBoard();

    // Gestion des événements (interaction entre le joueur et le plateau)
    void handleMouseClick(const sf::Vector2i& mousePosition);
    void selectPawn(const sf::Vector2i& mousePosition);
    void movePawn(const sf::Vector2i& mousePosition);
    void moveSelectedPawnTo(int targetRow, int targetCol);
    bool isValidSquare(int row, int col) const;

    // Gestion des deplacement d'un pion normal
    bool areMovesPossible(const Square& pawnSquare) const;
    void highlightPossibleMoves(const Square& currentSquare);
    void highlightSelectablePawns(int playerIndex);

    // Gestion des prises d'un pion normal
    bool areJumpsPossibleForPlayer(int playerIndex) const;
    bool areJumpsPossible(const Square& currentSquare) const;
    void highlightPossibleJumps(const Square& currentSquare);

    // Passage d'un pion normal en dame
    void promoteToDame(int row, int col);

    // Gestion des deplacements de la dame
    void checkDiagonalMoves(const Square& currentSquare, int rowIncrement, int colIncrement);
    void highlightPossibleMovesForDame(const Square& currentSquare);

    // Gestion des prises de la dame
    bool areJumpsPossibleForDame(const Square& currentSquare) const;
    bool isJumpPossibleForDame(const Square& currentSquare, int directionRow, int directionCol) const;
    void highlightPossibleJumpsForDame(const Square& currentSquare);

    // Logique principale du jeu
    void mainGameLoop(sf::RenderWindow& window);
    bool isPlayerOutOfPawns(int playerIndex) const;
    bool isGameOver() const;

    // Affichage des boutons et des informations des joueurs en sfml
    void drawBackButton(sf::RenderWindow& window) const;
    void drawConfirmButton(sf::RenderWindow& window) const;
    bool isConfirmButtonClicked(const sf::RenderWindow& window) const;
    void drawGameInfo(sf::RenderWindow& window) const;
    void drawRestartButton(sf::RenderWindow& window) const;
};

