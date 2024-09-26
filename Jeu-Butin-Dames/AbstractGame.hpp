
#ifndef ABSTRACTGAME_HPP
#define ABSTRACTGAME_HPP

#include <SFML/Graphics.hpp>

class AbstractGame {
public:
    virtual ~AbstractGame() = default;
    virtual void run(sf::RenderWindow& window) = 0;
    virtual bool hasReturnToMenuRequested() const = 0;

protected:
    virtual void handleWindowEvents(sf::RenderWindow& window) = 0;
    virtual void updateDisplay(sf::RenderWindow& window) = 0;
};

#endif 
