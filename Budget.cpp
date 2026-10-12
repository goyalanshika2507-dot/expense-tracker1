#include "Budget.h"

Budget::Budget()
    : category(""), limitAmount(0.0)
{
}

Budget::Budget(string cat, double limit)
    : category(cat), limitAmount(limit)
{
}

string Budget::getCategory() const
{
    return category;
}

double Budget::getLimitAmount() const
{
    return limitAmount;
}

void Budget::setCategory(const string& cat)
{
    category = cat;
}

void Budget::setLimitAmount(double limit)
{
    limitAmount = limit;
}

double Budget::getSpent(const vector<Transaction*>& transactions) const
{
    double spent = 0.0;
    for (const auto* t : transactions)
    {
        if (t && t->getType() == "Expense")
        {
            if (category == "Overall" || category.empty() || t->getCategoryOrSource() == category)
            {
                spent += t->getAmount();
            }
        }
    }
    return spent;
}

double Budget::getRemaining(const vector<Transaction*>& transactions) const
{
    double spent = getSpent(transactions);
    return limitAmount - spent;
}

bool Budget::isExceeded(const vector<Transaction*>& transactions) const
{
    if (limitAmount <= 0) return false;
    return getSpent(transactions) > limitAmount;
}

double Budget::getPercentageUsed(const vector<Transaction*>& transactions) const
{
    if (limitAmount <= 0) return 0.0;
    double pct = (getSpent(transactions) / limitAmount) * 100.0;
    return pct;
}
