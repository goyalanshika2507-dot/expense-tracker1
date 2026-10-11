#include "User.h"

User::User()
{
    userId = "";
    username = "";
    passwordHash = "";
}

User::User(string userId, string username, string passwordHash)
{
    this->userId = userId;
    this->username = username;
    this->passwordHash = passwordHash;
}

string User::getUserId() const
{
    return userId;
}

string User::getUsername() const
{
    return username;
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

bool User::checkPasswordHash(string passwordHash) const
{
    return this->passwordHash == passwordHash;
}