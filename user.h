#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User
{
private:
    string userId;
    string username;
    string passwordHash;
    string salt;

public:
    User();

    User(string userId, string username, string passwordHash, string salt = "");

    string getUserId() const;
    string getUsername() const;
    string getPasswordHash() const;
    string getSalt() const;

    void setUserId(string userId);
    void setUsername(string username);
    void setPasswordHash(string passwordHash);
    void setSalt(string salt);

    bool checkPasswordHash(string passwordHash) const;
};

#endif