#ifndef EXPENSE_H
#define EXPENSE_H

#include "Transaction.h"
#include <string>
using namespace std;

class Expense : public Transaction
{
private:
    string category;

public:
    Expense(int id, double amt, string d, string c);

    void display() override;
};

#endif