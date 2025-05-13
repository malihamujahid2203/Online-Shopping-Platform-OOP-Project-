#ifndef admin_h
#define admin_h
#include "user.h"
#include <iostream>
using namespace std;

class AdminUser : public User {
private:
    int adminCode;

public:
    AdminUser(const char* uname, const char* pass, int code) : User(uname, pass) {
        adminCode = code;
    }

    void displayAdminPanel() const {
        cout << "\n--- Admin Panel ---\n";
        cout << "Welcome, Admin " << getUsername() << "\n";
        cout << "Admin Code: " << adminCode << "\n";
        cout << "(You could add features here like: Add/Remove Product, View All Orders, etc.)\n";
    }
};

#endif
