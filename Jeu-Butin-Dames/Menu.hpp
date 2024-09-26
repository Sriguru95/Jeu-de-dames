#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu(sf::RenderWindow& window);
    ~Menu();

    void display();
    int handleEvents();
    int getSelectedOption() const;
    void resetSelectedOption();

private:
    void setupText(sf::Text& text, const std::string& label, float yPos, const sf::Color& color);
    void handleMouseClick(int mouseX, int mouseY);
    void highlightOption(sf::Text& option, const sf::Vector2i& mousePos);

    sf::RenderWindow& window;
    int selectedOption;
    sf::Font font;
    sf::Text title;
    sf::Text options[4];
    sf::RectangleShape optionBackgrounds[4];

    sf::Color buttonColor{30, 30, 30};
    sf::Color hoverColor{60, 60, 60};
};

#endif 
