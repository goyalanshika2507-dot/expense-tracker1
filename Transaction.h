#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include <string>
using namespace std;

class Transaction
{
protected:
    int transactionID;
    double amount;
    string date;

public:
    Transaction(int id, double amt, string d);

    virtual void display();

    virtual ~Transaction() = default;
};

#endif