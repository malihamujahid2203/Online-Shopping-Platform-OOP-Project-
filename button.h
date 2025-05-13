#ifndef button_h
#define button_h
#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <SFML/Window/Event.hpp>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    static sf::Font font;
    std::function<void()> onClick;
    bool isHovered = false;

public:
    Button(float x, float y, float width, float height,
               const std::string& text, sf::Font& font,
               std::function<void()> onClickFunc);

    void render(sf::RenderWindow& window);
    void update(const sf::Vector2f& mousePos);
    void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos);
    float getX() const { return shape.getPosition().x; }
    float getY() const { return shape.getPosition().y; }
    
    static bool loadFont(const std::string& filename);

};



#endif
