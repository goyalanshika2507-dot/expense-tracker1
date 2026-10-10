#ifndef INCOME_H
#define INCOME_H

#include "Transaction.h"
#include <string>
using namespace std;

class Income : public Transaction
{
private:
    string source;

public:
    Income(int id, double amt, string d, string s);

    void display() override;
};

#endif