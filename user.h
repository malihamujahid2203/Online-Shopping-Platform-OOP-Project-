#ifndef user_h
#define user_h
#include <string>
using namespace std;

class User {
private:
    char username[30];
    char password[30];

public:
    User() {
        strcpy(username, "guest");
        strcpy(password, "123");
    }

    User(const char* uname, const char* pass) {
        strcpy(username, uname);
        strcpy(password, pass);
    }

    bool login(const char* uname, const char* pass) const {
        return (strcmp(uname, username) == 0 && strcmp(pass, password) == 0);
    }

    void displayAccount() const {
        cout << "Account created for: " << username << endl;
    }
    
    const char* getUsername() const {
        return username;
    }
};


#endif
