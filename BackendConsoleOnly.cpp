#include <iostream>
#include <cstring>
#include "shop.h"
#include "cart.h"
#include "user.h"
#include "order.h"
#include "button.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

using namespace std;

void displayMainMenu() {
    cout << "\n--- Main Menu ---\n";
    cout << "1. View All Products\n";
    cout << "2. Search Products by Name\n";
    cout << "3. Filter Products by Category\n";
    cout << "4. Search Products by Price Range\n";
    cout << "5. View Cart\n";
    cout << "6. Edit Cart\n";
    cout << "7. Checkout\n";
    cout << "8. Exit\n";
    cout << "Choose an option: ";
}

void editCart(Cart &cart) {
    int option, pid, qty;
    cout << "\n--- Edit Cart ---\n";
    cout << "1. Remove Item\n";
    cout << "2. Update Quantity\n";
    cout << "Choice: ";
    cin >> option;

    cout << "Enter Product ID: ";
    cin >> pid;

    if (option == 1) {
        cart.removeFromCart(pid);
    } else if (option == 2) {
        cout << "Enter new quantity: ";
        cin >> qty;
        cart.updateQuantity(pid, qty);
    } else {
        cout << "Invalid choice.\n";
    }
}


enum class AppScreen { Shop, Cart, Checkout, Confirm };

struct AppState {
    AppScreen currentScreen = AppScreen::Shop;
    sf::Font font;
    Cart cart;
    std::string username;
    std::string location;
};

sf::Text createText(const std::string& str, sf::Font& font, int size, float x, float y) {
    sf::Text text(font, sf::String(str));
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::Black);
    text.setPosition(sf::Vector2f(x, y));
    return text;
}

void renderShop(sf::RenderWindow& window, AppState& state, std::vector<Button>& buttons, const std::vector<Product*>& products) {
    buttons.clear();
    float y = 80.f;

    for (auto* product : products) {
        std::string info = std::string(product->getName()) + " - $" + std::to_string(product->getPrice());
        window.draw(createText(info, state.font, 20, 50, y));

        buttons.emplace_back(400, y, 150, 30, "Add to Cart", state.font, [product, &state]() {
            state.cart.addToCart(product, 1);
        });

        y += 50;
    }

    buttons.emplace_back(50, y, 150, 40, "Go to Cart", state.font, [&state]() {
        state.currentScreen = AppScreen::Cart;
    });
}

void renderCart(sf::RenderWindow& window, AppState& state, std::vector<Button>& buttons) {
    buttons.clear();
    float y = 80.f;

    if (state.cart.isEmpty()) {
        window.draw(createText("Cart is empty.", state.font, 22, 50, y));
    } else {
        for (int i = 0; i < state.cart.getItemCount(); ++i) {
            Product* p = state.cart.getItem(i);
            int qty = state.cart.getQuantity(i);
            std::string line = std::string(p->getName()) + " x" + std::to_string(qty);
            window.draw(createText(line, state.font, 20, 50, y));
            y += 30;
        }
        y += 10;
        window.draw(createText("Total: $" + std::to_string(state.cart.getTotal()), state.font, 20, 50, y));
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
        state.cart.clear(); // this method must exist
        state.currentScreen = AppScreen::Shop;
    });
}




int main() {
    Shop shop;
    Cart cart;
    User savedUser("user", "pass");
    User currentUser;
    bool loggedIn = false;

    int choice;
    while (true) {
        displayMainMenu();
        cin >> choice;

        if (choice == 1) {
            shop.displayProducts();
            int id, qty;
            cout << "Enter Product ID to add to cart (0 to skip): ";
            cin >> id;
            if (id != 0) {
                Product* p = shop.getProductByID(id);
                if (p) {
                    cout << "Enter quantity: ";
                    cin >> qty;
                    cart.addToCart(p, qty);
                }
            }
        } else if (choice == 2) {
            char keyword[50];
            cout << "Enter keyword: ";
            cin >> keyword;
            shop.searchByName(keyword);
        } else if (choice == 3) {
            char category[30];
            cout << "Enter category (e.g., Electronics, Clothing): ";
            cin >> category;
            shop.displayByCategory(category);
        } else if (choice == 4) {
            double min, max;
            cout << "Enter min price: ";
            cin >> min;
            cout << "Enter max price: ";
            cin >> max;
            shop.searchByPrice(min, max);
        } else if (choice == 5) {
            cart.viewCart();
        } else if (choice == 6) {
            cart.viewCart();
            editCart(cart);
        } else if (choice == 7) {
            if (cart.isEmpty()) {
                cout << "Your cart is empty.\n";
                continue;
            }

            double total = cart.getTotal();
            cout << "\nSubtotal: $" << total << "\n";
            int opt;
            cout << "1. Create Account\n2. Login\n3. Guest Checkout\nChoice: ";
            cin >> opt;

            if (opt == 1) {
                char uname[30], pass[30];
                cout << "New username: ";
                cin >> uname;
                cout << "New password: ";
                cin >> pass;
                currentUser = User(uname, pass);
                loggedIn = true;
            } else if (opt == 2) {
                char uname[30], pass[30];
                cout << "Username: ";
                cin >> uname;
                cout << "Password: ";
                cin >> pass;
                if (savedUser.login(uname, pass)) {
                    cout << "Login successful.\n";
                    currentUser = savedUser;
                    loggedIn = true;
                } else {
                    cout << "Login failed.\n";
                    continue;
                }
            } else {
                cout << "Proceeding as guest.\n";
            }

            char location[50];
            cout << "Enter shipping city: ";
            cin >> location;

            Order order(cart, location);
            order.displayReceipt();

            if (loggedIn) {
                order.saveOrderHistory(currentUser.getUsername());
            }

            // Optional rating system
            char rateOpt;
            cout << "\nWould you like to rate the products? (y/n): ";
            cin >> rateOpt;
            if (rateOpt == 'y' || rateOpt == 'Y') {
                order.collectRatings();
            }

            cout << "\nThank you for shopping with us!\n";
            break;
        } else if (choice == 8) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }

    return 0;
}
