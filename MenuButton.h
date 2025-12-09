#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    sf::RectangleShape rect;
    sf::Text text;

    Button(float x, float y, float w, float h,
           const std::string& label,
           sf::Font& font,
           sf::Color bgColor = sf::Color(121, 0, 219),
           sf::Color textColor = sf::Color::White)
    {
        rect.setPosition(x, y);
        rect.setSize({w, h});
        rect.setFillColor(bgColor);
        rect.setOutlineThickness(2);
        rect.setOutlineColor(sf::Color::White);

        text.setFont(font);
        text.setString(label);
        text.setFillColor(textColor);
        text.setCharacterSize(40);

        // center text
        sf::FloatRect tb = text.getLocalBounds();
        text.setOrigin(tb.width/2, tb.height/2);
        text.setPosition(x + w/2, y + h/2 -5);
    }

    bool isHovered(sf::RenderWindow& win) {
        sf::Vector2i mouse = sf::Mouse::getPosition(win);
        return rect.getGlobalBounds().contains(mouse.x, mouse.y);
    }

    bool isClicked(sf::RenderWindow& win) {
        return isHovered(win) && sf::Mouse::isButtonPressed(sf::Mouse::Left);
    }

    bool isMouseOver(sf::Vector2f mousePos) {
        return rect.getGlobalBounds().contains(mousePos);
    }
    

    void draw(sf::RenderWindow& win) {
        win.draw(rect);
        win.draw(text);
    }
};
