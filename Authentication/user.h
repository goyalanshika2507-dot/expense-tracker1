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

public:
    User();

    User(string userId, string username, string passwordHash);

    string getUserId() const;
    string getUsername() const;

    void setUserId(string userId);
    void setUsername(string username);
    void setPasswordHash(string passwordHash);

    bool checkPasswordHash(string passwordHash) const;
};

#endif