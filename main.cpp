#include <iostream>
#include <fstream>
#include <cstring>
#include "shop.h"
#include "cart.h"
#include "user.h"
#include "order.h"
#include "button.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <optional>

using namespace std;

enum class AppScreen { Shop, Cart, Checkout, Confirm };

struct AppState {
    AppScreen currentScreen = AppScreen::Shop;
    sf::Font font;
    Cart cart;
    std::string username;
    std::string location;
    std::string cartMessage;
    sf::Clock cartMessageTimer;
};

sf::Text createText(const std::string& str, sf::Font& font, int size, float x, float y, sf::Color color = sf::Color::Black) {
    sf::Text text(font, sf::String(str));
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(sf::Vector2f(x, y));
    return text;
}

void renderShop(sf::RenderWindow& window, AppState& state, std::vector<Button>& buttons, const std::vector<Product*>& products) {
    buttons.clear();
    float y = 80.f;

    window.draw(createText("Available Products", state.font, 28, 50, 20));

    for (auto* product : products) {
        sf::RectangleShape box(sf::Vector2f(700, 60));
        box.setPosition(sf::Vector2f(30.f, y));
        box.setFillColor(sf::Color(240, 240, 255));
        box.setOutlineColor(sf::Color::Blue);
        box.setOutlineThickness(1);
        window.draw(box);

        std::string info = std::string(product->getName()) + " - $" + std::to_string(product->getPrice());
        window.draw(createText(info, state.font, 20, 40, y + 10));

        buttons.emplace_back(600, y + 15, 120, 30, "Add to Cart", state.font, [product, &state]() {
            state.cart.addToCart(product, 1);
            state.cartMessage = std::string(product->getName()) + " added to cart!";
            state.cartMessageTimer.restart();
        });

        y += 70;
    }

    buttons.emplace_back(50, y + 10, 150, 40, "Go to Cart", state.font, [&state]() {
        state.currentScreen = AppScreen::Cart;
    });

    if (!state.cartMessage.empty() && state.cartMessageTimer.getElapsedTime().asSeconds() < 2.5f) {
        window.draw(createText(state.cartMessage, state.font, 20, 50, y + 70, sf::Color::Green));
    }
}

void renderCart(sf::RenderWindow& window, AppState& state, std::vector<Button>& buttons) {
    buttons.clear();
    float y = 80.f;

    window.draw(createText("Your Cart", state.font, 26, 50, 20));

    if (state.cart.isEmpty()) {
        window.draw(createText("Cart is empty.", state.font, 22, 50, y));
    } else {
        for (int i = 0; i < state.cart.getItemCount(); ++i) {
            Product* p = state.cart.getItem(i);
            int qty = state.cart.getQuantity(i);
            std::string line = std::string(p->getName()) + " x" + std::to_string(qty);
            sf::RectangleShape lineBox(sf::Vector2f(700, 30));
            lineBox.setPosition(sf::Vector2f(30.f, y));
            lineBox.setFillColor(sf::Color(240, 255, 240));
            lineBox.setOutlineColor(sf::Color::Black);
            lineBox.setOutlineThickness(1);
            window.draw(lineBox);
            window.draw(createText(line, state.font, 20, 40, y + 5));
            y += 40;
        }
        y += 10;
        window.draw(createText("Total: $" + std::to_string(state.cart.getTotal()), state.font, 20, 50, y));

        std::ofstream cartFile("cart.txt");
        state.cart.printItemsToFile(cartFile);
        cartFile.close();

        std::ofstream receipt("receipt.txt");
        receipt << "--- Receipt ---\n";
        state.cart.printItemsToFile(receipt);
        receipt << "\nTotal: $" << state.cart.getTotal() << "\n";
        receipt.close();
    }

    y += 50;
    buttons.emplace_back(50, y, 150, 40, "Checkout", state.font, [&state]() {
        state.currentScreen = AppScreen::Checkout;
    });
    buttons.emplace_back(220, y, 150, 40, "Back", state.font, [&state]() {
        state.currentScreen = AppScreen::Shop;
    });
}

void renderCheckout(sf::RenderWindow& window, AppState& state, std::vector<Button>& buttons) {
    buttons.clear();
    window.draw(createText("Choose Checkout Option:", state.font, 24, 50, 60));

    buttons.emplace_back(50, 120, 180, 40, "Guest", state.font, [&state]() {
        state.username = "Guest";
        state.location = "AutoTown";
        state.currentScreen = AppScreen::Confirm;
    });

    buttons.emplace_back(50, 180, 180, 40, "Login", state.font, [&state]() {
        state.username = "User123";
        state.location = "UserCity";
        state.currentScreen = AppScreen::Confirm;
    });

    buttons.emplace_back(50, 240, 180, 40, "Create Account", state.font, [&state]() {
        state.username = "NewUser";
        state.location = "NewVille";
        state.currentScreen = AppScreen::Confirm;
    });

    buttons.emplace_back(50, 300, 150, 40, "Back", state.font, [&state]() {
        state.currentScreen = AppScreen::Cart;
    });
}

void renderConfirm(sf::RenderWindow& window, AppState& state, std::vector<Button>& buttons) {
    buttons.clear();
    window.draw(createText("Thank you, " + state.username + "!", state.font, 26, 50, 100));
    window.draw(createText("Your order will ship to: " + state.location, state.font, 20, 50, 150));
    window.draw(createText("Total: $" + std::to_string(state.cart.getTotal()), state.font, 20, 50, 190));

    buttons.emplace_back(50, 250, 180, 40, "Return to Shop", state.font, [&state]() {
        state.cart.clear();
        state.currentScreen = AppScreen::Shop;
    });
}

int main() {
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "Online Shop");
    AppState appState;

    if (!appState.font.openFromFile("resources/arial.ttf")) {
        std::cerr << "Font failed to load.\n";
        return 1;
    }

    Product p1(1, "Keyboard", 29.99, "Electronics");
    Product p2(2, "Mouse", 19.99, "Electronics");
    Product p3(3, "Monitor", 199.99, "Displays");
    Product p4(4, "Laptop", 999.99, "Computers");

    std::vector<Product*> products = { &p1, &p2, &p3, &p4 };
    std::vector<Button> buttons;
    std::vector<sf::Texture> productTextures;
    std::vector<sf::Sprite> productSprites;

    std::vector<std::string> imagePaths = {
        "resources/keyboard.jpg",
        "resources/mouse.jpg",
        "resources/monitor.jpg",
        "resources/laptop.jpg"
    };

    for (const auto& path : imagePaths) {
        sf::Texture texture;
        if (texture.loadFromFile(path)) {
            productTextures.push_back(texture);
            sf::Sprite sprite(productTextures.back());
            sprite.setScale(sf::Vector2f(0.25f, 0.25f));
            productSprites.push_back(sprite);
        }
    }

    while (window.isOpen()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        std::optional<sf::Event> event = window.pollEvent();

        while (event) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            for (auto& b : buttons) b.handleEvent(*event, mousePos);
            event = window.pollEvent();
        }

        for (auto& b : buttons) b.update(mousePos);

        window.clear(sf::Color(200, 255, 200));

        switch (appState.currentScreen) {
            case AppScreen::Shop: renderShop(window, appState, buttons, products); break;
            case AppScreen::Cart: renderCart(window, appState, buttons); break;
            case AppScreen::Checkout: renderCheckout(window, appState, buttons); break;
            case AppScreen::Confirm: renderConfirm(window, appState, buttons); break;
        }

        for (auto& b : buttons) {
            sf::RectangleShape background(sf::Vector2f(150, 40));
            background.setPosition(sf::Vector2f(b.getX(), b.getY()));
            background.setFillColor(sf::Color(0, 100, 0));
            background.setOutlineColor(sf::Color::Black);
            background.setOutlineThickness(2);
            window.draw(background);
            b.render(window);
        }

        float yOffset = 50.f;
        for (auto& sprite : productSprites) {
            sprite.setPosition(sf::Vector2f(600, yOffset));
            window.draw(sprite);
            yOffset += 100.f;
        }

        std::ofstream receipt("receipt.txt");
        receipt << "Thank you for your purchase!\n";
        appState.cart.printItemsToFile(receipt);
        receipt << "Total: $" << appState.cart.getTotal() << std::endl;
        receipt.close();

        sf::Text inputLabel(appState.font);
        inputLabel.setFont(appState.font);
        inputLabel.setString("Enter username:");
        inputLabel.setCharacterSize(20);
        inputLabel.setPosition(sf::Vector2f(50.f, 500.f));
        inputLabel.setFillColor(sf::Color::Black);

        sf::RectangleShape inputBox;
        inputBox.setSize(sf::Vector2f(200.f, 30.f));
        inputBox.setPosition(sf::Vector2f(200.f, 495.f));
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineColor(sf::Color::Black);
        inputBox.setOutlineThickness(1.f);
        window.draw(inputBox);

        window.display();
    }

    return 0;
}
