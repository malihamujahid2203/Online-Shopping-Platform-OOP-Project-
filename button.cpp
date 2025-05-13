#include "button.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

sf::Font Button::font;

Button::Button(float x, float y, float width, float height,
               const std::string& label, sf::Font& font,
               std::function<void()> onClickFunc)
    : shape(sf::Vector2f(width, height)),
      text(font, sf::String(label)), 
      onClick(onClickFunc),
      isHovered(false)
{
    shape.setPosition(sf::Vector2f(x, y));
    shape.setFillColor(sf::Color::Blue);

    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(x + 10, y + 8));
}

void Button::render(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

void Button::update(const sf::Vector2f& mousePos) {
    isHovered = shape.getGlobalBounds().contains(mousePos);
    shape.setFillColor(isHovered ? sf::Color::Cyan : sf::Color::Blue);
}

void Button::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) {
    if (event.is<sf::Event::MouseButtonPressed>()) {
        auto btn = event.getIf<sf::Event::MouseButtonPressed>();
        if (btn && btn->button == sf::Mouse::Button::Left) {
            if (shape.getGlobalBounds().contains(mousePos)) {
                if (onClick) onClick();
            }
        }
    }
}
