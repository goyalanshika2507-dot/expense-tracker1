#include "Income.h"

Income::Income(int id, double amt, string d, string s)
    : Transaction(id, amt, d)
{
    source = s;
}

void Income::display()
{
    cout << "\n--- Income Details ---" << endl;

    Transaction::display();

    cout << "Income Source: " << source << endl;
}