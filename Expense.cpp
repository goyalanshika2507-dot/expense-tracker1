#include "Expense.h"

Expense::Expense(int id, double amt, string d, string c)
    : Transaction(id, amt, d)
{
    category = c;
}

void Expense::display()
{
    cout << "\n--- Expense Details ---" << endl;

    Transaction::display();

    cout << "Expense Category: " << category << endl;
}

string Expense::getCategory() const
{
    return category;
}

void Expense::setCategory(const string& c)
{
    category = c;
}

string Expense::getType() const
{
    return "Expense";
}

string Expense::getCategoryOrSource() const
{
    return category;
}

void Expense::setCategoryOrSource(const string& val)
{
    category = val;
}
