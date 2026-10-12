#include "user.h"

User::User()
{
    userId = "";
    username = "";
    passwordHash = "";
    salt = "";
}

User::User(string userId, string username, string passwordHash, string salt)
{
    this->userId = userId;
    this->username = username;
    this->passwordHash = passwordHash;
    this->salt = salt;
}

string User::getUserId() const
{
    return userId;
}

string User::getUsername() const
{
    return username;
}

string User::getPasswordHash() const
{
    return passwordHash;
}

string User::getSalt() const
{
    return salt;
}

void User::setUserId(string userId)
{
    this->userId = userId;
}

void User::setUsername(string username)
{
    this->username = username;
}

void User::setPasswordHash(string passwordHash)
{
    this->passwordHash = passwordHash;
}

void User::setSalt(string salt)
{
    this->salt = salt;
}

bool User::checkPasswordHash(string passwordHash) const
{
    return this->passwordHash == passwordHash;
}