#include "Menu.hpp"

Menu::Menu(sf::RenderWindow& window)
    : window{window}, selectedOption{-1}, font{}, title{}, options(), optionBackgrounds() {
    font.loadFromFile("arial.ttf");

    title.setFont(font);
    title.setString("Menu");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition(window.getSize().x / 2.0f - title.getGlobalBounds().width / 2.0f, 50);

    const std::string labels[4] = {"Butin", "Dames", "Safari", "Quitter"};
    for (int i = 0; i < 4; ++i) {
        setupText(options[i], labels[i], 290 + i * 120, sf::Color::White);
        optionBackgrounds[i].setSize(sf::Vector2f(300.0f, 70.0f));
        optionBackgrounds[i].setPosition((window.getSize().x - 300.0f) / 2, 290 + i * 120);
        optionBackgrounds[i].setFillColor(buttonColor);
        optionBackgrounds[i].setOutlineColor(sf::Color::White);
        optionBackgrounds[i].setOutlineThickness(2.0f);
    }
}

Menu::~Menu() {
    // Éventuelles opérations de nettoyage
}

int Menu::getSelectedOption() const {
    return selectedOption;
}

void Menu::resetSelectedOption() {
    selectedOption = -1;
}

void Menu::display() {
    window.clear();
    window.draw(title);
    for (int i = 0; i < 4; ++i) {
        window.draw(optionBackgrounds[i]);
        window.draw(options[i]);
    }
    window.display();
}

int Menu::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left) {
            handleMouseClick(event.mouseButton.x, event.mouseButton.y);
        } else if (event.type == sf::Event::MouseMoved) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            for (int i = 0; i < 4; ++i) {
                highlightOption(options[i], mousePos);
            }
        }
    }

    if (!window.isOpen()) {
        selectedOption = 3; 
    }

    return selectedOption;
}

void Menu::handleMouseClick(int mouseX, int mouseY) {
    for (int i = 0; i < 4; ++i) {
        if (optionBackgrounds[i].getGlobalBounds().contains(static_cast<float>(mouseX), static_cast<float>(mouseY))) {
            selectedOption = i;
            break;
        }
    }
}

void Menu::highlightOption(sf::Text& option, const sf::Vector2i& mousePos) {
    int index = &option - &options[0]; // Calcule l'indice de l'option dans le tableau
    if (optionBackgrounds[index].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        optionBackgrounds[index].setFillColor(hoverColor);
    } else {
        optionBackgrounds[index].setFillColor(buttonColor);
    }
}

void Menu::setupText(sf::Text& text, const std::string& label, float yPos, const sf::Color& color) {
    text.setString(label);
    text.setFont(font);
    text.setCharacterSize(30);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(window.getSize().x / 2.0f, yPos + 70.0f / 2.0f);
    text.setFillColor(color);
}