#ifndef shop_h
#define shop_h
#include "product.h"
#include <iostream>
#include <cstring>
using namespace std;

class Shop {
private:
    Product products[10];
    int productCount;

public:
    Shop() {
        products[0] = Product(1, "Laptop", 85000, "Electronics");
        products[1] = Product(2, "Smartphone", 50000, "Electronics");
        products[2] = Product(3, "Headphones", 4000, "Electronics");
        products[3] = Product(4, "T-Shirt", 1500, "Clothing");
        products[4] = Product(5, "Shoes", 3500, "Clothing");
        productCount = 5;
    }

    void displayProducts() const {
        cout << "\n--- Product List ---\n";
        for (int i = 0; i < productCount; i++) {
            products[i].display();
        }
    }

    void displayByCategory(const char* category) const {
        cout << "\n--- Products in " << category << " ---\n";
        for (int i = 0; i < productCount; i++) {
            if (strcmp(products[i].getCategory(), category) == 0) {
                products[i].display();
            }
        }
    }

    void searchByName(const char* keyword) const {
        cout << "\n--- Search Results ---\n";
        for (int i = 0; i < productCount; i++) {
            if (strstr(products[i].getName(), keyword)) {
                products[i].display();
            }
        }
    }

    void searchByPrice(double min, double max) const {
        cout << "\n--- Price Range: $" << min << " to $" << max << " ---\n";
        for (int i = 0; i < productCount; i++) {
            double p = products[i].getPrice();
            if (p >= min && p <= max) {
                products[i].display();
            }
        }
    }

    Product* getProductByID(int id) {
        for (int i = 0; i < productCount; i++) {
            if (products[i].getID() == id)
                return &products[i];
        }
        return nullptr;
    }
};

#endif
