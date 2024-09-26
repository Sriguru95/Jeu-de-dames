#include "AbstractGame.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class ButinGame : public AbstractGame {
public:
    ButinGame(const std::vector<std::string>& playerNames);
    ~ButinGame();

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
    mutable sf::RectangleShape confirmButton;
    bool yellowPawnMoved;
    bool gameEnded;
    bool restartRequested;
    bool returnToMenuRequested;
    mutable sf::RectangleShape restartButton;
    mutable sf::RectangleShape backButton;

    void resetGame();

    void createBoard();

    // Gestion des événements
    void handleMouseClick(const sf::Vector2i& mousePosition);
    void selectPawn(const sf::Vector2i& mousePosition);
    void movePawn(const sf::Vector2i& mousePosition);
    void moveSelectedPawnTo(int targetRow, int targetCol);
    bool isValidSquare(int row, int col) const;

    // Logique principale du jeu
    void prepareGame(sf::RenderWindow& window);
    void mainGameLoop(sf::RenderWindow& window);
    bool isGameOver() const;
    int getWinningPlayerIndex() const;

    // Affichage
    void drawBackButton(sf::RenderWindow& window) const;
    void drawConfirmButton(sf::RenderWindow& window) const;
    bool isConfirmButtonClicked(const sf::RenderWindow& window) const;
    void drawGameInfo(sf::RenderWindow& window) const;
    void drawRestartButton(sf::RenderWindow& window) const;
};

