#include "Transaction.h"

Transaction::Transaction(int id, double amt, string d)
{
    transactionID = id;
    amount = amt;
    date = d;
}

void Transaction::display()
{
    cout << "Transaction ID: " << transactionID << endl;
    cout << "Amount: Rs. " << amount << endl;
    cout << "Date: " << date << endl;
}

int Transaction::getId() const
{
    return transactionID;
}

double Transaction::getAmount() const
{
    return amount;
}

string Transaction::getDate() const
{
    return date;
}

void Transaction::setAmount(double amt)
{
    amount = amt;
}

void Transaction::setDate(const string& d)
{
    date = d;
}
