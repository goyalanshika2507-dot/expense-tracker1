#include <iostream>
#include "authentication.h"

using namespace std;

Authentication::Authentication()
{
    loggedIn = false;
}

bool Authentication::validatePassword(string password) const
{
    return password.length() >= 8;
}

bool Authentication::registerUser(string username, string password)
{
    if (username.empty())
    {
        cout << "Username cannot be empty!\n";
        return false;
    }

    if (!validatePassword(password))
    {
        cout << "Password must contain at least 8 characters!\n";
        return false;
    }

    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].getUsername() == username)
        {
            cout << "Username already exists!\n";
            return false;
        }
    }

    User newUser(username, username, password);

    users.push_back(newUser);

    cout << "Registration successful!\n";
    return true;
}

bool Authentication::loginUser(string username, string password)
{
    if (loggedIn)
    {
        cout << "A user is already logged in!\n";
        return false;
    }

    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].getUsername() == username)
        {
            if (users[i].checkPasswordHash(password))
            {
                currentUser = users[i];
                loggedIn = true;

                cout << "Login successful!\n";
                return true;
            }

            cout << "Incorrect password!\n";
            return false;
        }
    }

    cout << "User not found!\n";
    return false;
}

void Authentication::logoutUser()
{
    if (!loggedIn)
    {
        cout << "No user is currently logged in!\n";
        return;
    }

    loggedIn = false;
    currentUser = User();

    cout << "Logout successful!\n";
}

User Authentication::getCurrentUser() const
{
    return currentUser;
}