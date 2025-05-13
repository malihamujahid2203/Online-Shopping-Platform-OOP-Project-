#ifndef product_h
#define product_h
#include <string>
#include <cstring>
using namespace std;

class Product {
private:
    int id;
    char name[50];
    double price;
    char category[30];
    int totalRating;
    int ratingCount;

public:
    Product() {
        id = 0;
        strcpy(name, "Unnamed");
        price = 0.0;
        strcpy(category, "General");
        totalRating = 0;
        ratingCount = 0;
    }

    Product(int id, const char* name, double price, const char* cat) {
        this->id = id;
        strcpy(this->name, name);
        this->price = price;
        strcpy(this->category, cat);
        totalRating = 0;
        ratingCount = 0;
    }

    void display() const {
        cout << "ID: " << id << " | " << name << " | Category: " << category
             << " | Price: $" << price;
        if (ratingCount > 0)
            cout << " | Rating: " << (double)totalRating / ratingCount << "/5";
        cout << endl;
    }

    void addRating(int stars) {
        if (stars >= 1 && stars <= 5) {
            totalRating += stars;
            ratingCount++;
        }
    }

    int getID() const { return id; }
    double getPrice() const { return price; }
    const char* getName() const { return name; }
    const char* getCategory() const { return category; }
};


#endif
