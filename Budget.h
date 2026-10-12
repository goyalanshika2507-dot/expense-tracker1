#ifndef BUDGET_H
#define BUDGET_H

#include <string>
#include <vector>
#include "Transaction.h"

using namespace std;

class Budget
{
private:
    string category;
    double limitAmount;

public:
    Budget();
    Budget(string cat, double limit);

    string getCategory() const;
    double getLimitAmount() const;

    void setCategory(const string& cat);
    void setLimitAmount(double limit);

    double getSpent(const vector<Transaction*>& transactions) const;
    double getRemaining(const vector<Transaction*>& transactions) const;
    bool isExceeded(const vector<Transaction*>& transactions) const;
    double getPercentageUsed(const vector<Transaction*>& transactions) const;
};

#endif
