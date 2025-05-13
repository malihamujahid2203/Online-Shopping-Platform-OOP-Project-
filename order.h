#ifndef order_h
#define order_h
#include "cart.h"
#include "discount.h"
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

class Order {
private:
    static int orderCounter;
    int orderID;
    Cart cart;
    char location[50];
    double shippingFee;
    Discount* discountStrategy;

public:
    Order(const Cart& c, const char* loc) : cart(c) {
        orderID = ++orderCounter;
        strcpy(location, loc);

        if (strcmp(location, "Lahore") == 0) shippingFee = 100;
        else if (strcmp(location, "Karachi") == 0) shippingFee = 200;
        else shippingFee = 250;

        double total = cart.getTotal();
        if (total >= 20000)
            discountStrategy = new PercentageDiscount(10); // 10% off
        else
            discountStrategy = new NoDiscount();
    }

    void displayReceipt() const {
        cout << "\n--- Order Summary ---\n";
        cart.viewCart();
        double subtotal = cart.getTotal();
        cout << "Subtotal: $" << subtotal << endl;
        double discount = discountStrategy->apply(cart.getTotal());
        if (discount > 0)
            cout << "Discount Applied: -$" << discount << endl;
        cout << "Shipping Fee: $" << shippingFee << endl;
        cout << "Final Total: $" << getFinalAmount() << endl;
        cout << "Order ID: ORD" << orderID << endl;
        cout << "Estimated Delivery: " << getDeliveryTime() << " day(s)\n";
    }

    double getFinalAmount() const {
        return cart.getTotal() - discountStrategy->apply(cart.getTotal()) + shippingFee;
    }

    int getDeliveryTime() const {
        if (strcmp(location, "Lahore") == 0)
            return 1;
        return 3;
    }

    void saveOrderHistory(const char* username) const {
        ofstream file;
        char filename[100];
        strcpy(filename, username);
        strcat(filename, "_orders.txt");

        file.open(filename, ios::app);
        file << "Order ID: ORD" << orderID << "\n";
        cart.printItemsToFile(file);
        file << "Total Paid: $" << getFinalAmount() << "\n\n";
        file.close();
    }

    void collectRatings() {
        char rateOpt;
        cout << "\nWould you like to rate the products? (y/n): ";
        cin >> rateOpt;

        if (rateOpt == 'y' || rateOpt == 'Y') {
            int count = cart.getItemCount();
            for (int i = 0; i < count; i++) {
                Product* item = cart.getItem(i);
                if (item) {
                    int rating;
                    cout << "Rate " << item->getName() << " (1-5): ";
                    cin >> rating;
                    item->addRating(rating);
                }
            }
        }
    }
    ~Order() {
        delete discountStrategy;
    }
};

int Order::orderCounter = 0;


#endif
#ifndef order_h
#define order_h
#include "cart.h"
#include "discount.h"
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

class Order {
private:
    static int orderCounter;
    int orderID;
    Cart cart;
    char location[50];
    double shippingFee;
    Discount* discountStrategy;

public:
    Order(const Cart& c, const char* loc) : cart(c) {
        orderID = ++orderCounter;
        strcpy(location, loc);

        if (strcmp(location, "Lahore") == 0) shippingFee = 100;
        else if (strcmp(location, "Karachi") == 0) shippingFee = 200;
        else shippingFee = 250;

        double total = cart.getTotal();
        if (total >= 20000)
            discountStrategy = new PercentageDiscount(10); // 10% off
        else
            discountStrategy = new NoDiscount();
    }

    void displayReceipt() const {
        cout << "\n--- Order Summary ---\n";
        cart.viewCart();
        double subtotal = cart.getTotal();
        cout << "Subtotal: $" << subtotal << endl;
        double discount = discountStrategy->apply(cart.getTotal());
        if (discount > 0)
            cout << "Discount Applied: -$" << discount << endl;
        cout << "Shipping Fee: $" << shippingFee << endl;
        cout << "Final Total: $" << getFinalAmount() << endl;
        cout << "Order ID: ORD" << orderID << endl;
        cout << "Estimated Delivery: " << getDeliveryTime() << " day(s)\n";
    }

    double getFinalAmount() const {
        return cart.getTotal() - discountStrategy->apply(cart.getTotal()) + shippingFee;
    }

    int getDeliveryTime() const {
        if (strcmp(location, "Lahore") == 0)
            return 1;
        return 3;
    }

    void saveOrderHistory(const char* username) const {
        ofstream file;
        char filename[100];
        strcpy(filename, username);
        strcat(filename, "_orders.txt");

        file.open(filename, ios::app);
        file << "Order ID: ORD" << orderID << "\n";
        cart.printItemsToFile(file);
        file << "Total Paid: $" << getFinalAmount() << "\n\n";
        file.close();
    }

    void collectRatings() {
        char rateOpt;
        cout << "\nWould you like to rate the products? (y/n): ";
        cin >> rateOpt;

        if (rateOpt == 'y' || rateOpt == 'Y') {
            int count = cart.getItemCount();
            for (int i = 0; i < count; i++) {
                Product* item = cart.getItem(i);
                if (item) {
                    int rating;
                    cout << "Rate " << item->getName() << " (1-5): ";
                    cin >> rating;
                    item->addRating(rating);
                }
            }
        }
    }
    ~Order() {
        delete discountStrategy;
    }
};

int Order::orderCounter = 0;


#endif
