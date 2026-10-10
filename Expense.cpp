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