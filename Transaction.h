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

    // Getters
    int getId() const;
    double getAmount() const;
    string getDate() const;

    // Pure virtual methods for runtime polymorphism
    virtual string getType() const = 0;
    virtual string getCategoryOrSource() const = 0;

    // Setters for editing records
    void setAmount(double amt);
    void setDate(const string& d);
    virtual void setCategoryOrSource(const string& val) = 0;
};

#endif
