#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

class Transaction {
protected:
    int transactionId;
    string username;
    double amount;
    string category;
    string date; // Format: YYYY-MM-DD
    string type; // "Income" or "Expense"

public:
    Transaction(int id, string user, double amt, string cat, string dt, string t);
    virtual ~Transaction();

    string getUsername() const;
    double getAmount() const;
    string getCategory() const;
    string getDate() const;
    string getType() const;
    int getId() const;

    virtual void displayDetails() const;
};

class Income : public Transaction {
public:
    Income(int id, string user, double amt, string cat, string dt);
    void displayDetails() const override;
};

class Expense : public Transaction {
public:
    Expense(int id, string user, double amt, string cat, string dt);
    void displayDetails() const override;
};

#endif