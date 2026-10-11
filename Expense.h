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

    string getCategory() const;
    void setCategory(const string& c);

    string getType() const override;
    string getCategoryOrSource() const override;
    void setCategoryOrSource(const string& val) override;
};

#endif
