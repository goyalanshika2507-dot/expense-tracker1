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

string Income::getSource() const
{
    return source;
}

void Income::setSource(const string& s)
{
    source = s;
}

string Income::getType() const
{
    return "Income";
}

string Income::getCategoryOrSource() const
{
    return source;
}

void Income::setCategoryOrSource(const string& val)
{
    source = val;
}
