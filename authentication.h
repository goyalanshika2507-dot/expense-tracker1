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

public:
    Authentication();

    bool validatePassword(string password) const;

    bool registerUser(string username, string password);

    bool loginUser(string username, string password);

    void logoutUser();

    User getCurrentUser() const;
};

#endif