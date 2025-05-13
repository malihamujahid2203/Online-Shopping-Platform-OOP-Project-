#ifndef cart_h
#define cart_h
#include <string>
#include "product.h"
#include <fstream>
using namespace std;

class Cart {
private:
    Product* items[50];
    int quantities[50];
    int itemCount;

public:
    Cart() {
        itemCount = 0;
    }

    void addToCart(Product* product, int quantity) {
        for (int i = 0; i < itemCount; i++) {
            if (items[i]->getID() == product->getID()) {
                quantities[i] += quantity;
                return;
            }
        }
        items[itemCount] = product;
        quantities[itemCount] = quantity;
        itemCount++;
    }

    void removeFromCart(int id) {
        for (int i = 0; i < itemCount; i++) {
            if (items[i]->getID() == id) {
                for (int j = i; j < itemCount - 1; j++) {
                    items[j] = items[j + 1];
                    quantities[j] = quantities[j + 1];
                }
                itemCount--;
                cout << "Item removed.\n";
                return;
            }
        }
        cout << "Item not found in cart.\n";
    }

    void updateQuantity(int id, int newQty) {
        for (int i = 0; i < itemCount; i++) {
            if (items[i]->getID() == id) {
                quantities[i] = newQty;
                return;
            }
        }
        cout << "Item not found in cart.\n";
    }

    void viewCart() const {
        if (itemCount == 0) {
            cout << "Cart is empty.\n";
            return;
        }

        cout << "\n--- Cart ---\n";
        for (int i = 0; i < itemCount; i++) {
            cout << items[i]->getName() << " x" << quantities[i]
                 << " = $" << items[i]->getPrice() * quantities[i] << endl;
        }
    }

    double getTotal() const {
        double total = 0;
        for (int i = 0; i < itemCount; i++) {
            total += items[i]->getPrice() * quantities[i];
        }
        return total;
    }

    bool isEmpty() const {
        return itemCount == 0;
    }

    int getItemCount() const { return itemCount; }

    Product* getItem(int index) const {
        return (index >= 0 && index < itemCount) ? items[index] : nullptr;
    }

    int getQuantity(int index) const {
        return (index >= 0 && index < itemCount) ? quantities[index] : 0;
    }

    void printItemsToFile(ofstream& file) const {
        for (int i = 0; i < itemCount; i++) {
            file << items[i]->getName() << " x" << quantities[i] << "\n";
        }
    }
    
    void clear() { itemCount = 0; }

};


#endif
