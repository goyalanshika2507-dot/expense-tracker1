#include <iostream>
#include <random>
#include <QCryptographicHash>
#include <QPasswordDigestor>
#include <QByteArray>
#include "authentication.h"
#include "FileManager.h"

using namespace std;

Authentication::Authentication()
{
    loggedIn = false;
    lastError = "";
    reloadUsers();
}

void Authentication::reloadUsers()
{
    FileManager::loadUsers(users);
}

string Authentication::generateSalt()
{
    static const char hexChars[] = "0123456789abcdef";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 15);
    string saltStr = "";
    for (int i = 0; i < 16; ++i)
    {
        saltStr += hexChars[dis(gen)];
    }
    return saltStr;
}

string Authentication::hashPassword(const string& password, const string& salt)
{
    QByteArray pw = QByteArray::fromStdString(password);
    QByteArray s = QByteArray::fromStdString(salt);
    // Standard PBKDF2 with HMAC-SHA-256 (10,000 rounds of key stretching, 32-byte derived key)
    QByteArray derived = QPasswordDigestor::deriveKeyPbkdf2(QCryptographicHash::Sha256, pw, s, 10000, 32);
    return derived.toHex().toStdString();
}

bool Authentication::validatePassword(string password) const
{
    return password.length() >= 8;
}

bool Authentication::registerUser(string username, string password)
{
    if (username.empty())
    {
        lastError = "Username cannot be empty!";
        cout << lastError << "\n";
        return false;
    }

    if (!validatePassword(password))
    {
        lastError = "Password must contain at least 8 characters!";
        cout << lastError << "\n";
        return false;
    }

    // Refresh users from storage before checking uniqueness
    reloadUsers();

    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i].getUsername() == username)
        {
            lastError = "Username already exists!";
            cout << lastError << "\n";
            return false;
        }
    }

    string salt = generateSalt();
    string hashed = hashPassword(password, salt);
    User newUser(username, username, hashed, salt);

    users.push_back(newUser);
    FileManager::saveUsers(users);

    lastError = "";
    cout << "Registration successful!\n";
    return true;
}

bool Authentication::loginUser(string username, string password)
{
    if (loggedIn)
    {
        lastError = "A user is already logged in!";
        cout << lastError << "\n";
        return false;
    }

    // Refresh users from storage
    reloadUsers();

    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i].getUsername() == username)
        {
            string salt = users[i].getSalt();
            bool passwordMatches = false;

            if (!salt.empty())
            {
                string computedHash = hashPassword(password, salt);
                passwordMatches = (users[i].getPasswordHash() == computedHash);
            }
            else
            {
                // Backward-compatibility: upgrade legacy plain-text password to salted hash
                if (users[i].checkPasswordHash(password))
                {
                    passwordMatches = true;
                    string newSalt = generateSalt();
                    users[i].setSalt(newSalt);
                    users[i].setPasswordHash(hashPassword(password, newSalt));
                    FileManager::saveUsers(users);
                }
            }

            if (passwordMatches)
            {
                currentUser = users[i];
                loggedIn = true;
                lastError = "";
                cout << "Login successful!\n";
                return true;
            }

            lastError = "Incorrect password!";
            cout << lastError << "\n";
            return false;
        }
    }

    lastError = "User not found!";
    cout << lastError << "\n";
    return false;
}

void Authentication::logoutUser()
{
    if (!loggedIn)
    {
        lastError = "No user is currently logged in!";
        cout << lastError << "\n";
        return;
    }

    loggedIn = false;
    currentUser = User();
    lastError = "";

    cout << "Logout successful!\n";
}

User Authentication::getCurrentUser() const
{
    return currentUser;
}

bool Authentication::isLoggedIn() const
{
    return loggedIn;
}

string Authentication::getLastError() const
{
    return lastError;
}