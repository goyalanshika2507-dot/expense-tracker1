#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <string>
#include <vector>
#include "user.h"

using namespace std;

class Authentication
{
private:
    User currentUser;
    bool loggedIn;
    vector<User> users;

    string lastError;

public:
    Authentication();

    static string hashPassword(const string& password, const string& salt);
    static string generateSalt();

    bool validatePassword(string password) const;

    bool registerUser(string username, string password);

    bool loginUser(string username, string password);

    void logoutUser();

    User getCurrentUser() const;
    bool isLoggedIn() const;
    string getLastError() const;
    void reloadUsers();
};

#endif