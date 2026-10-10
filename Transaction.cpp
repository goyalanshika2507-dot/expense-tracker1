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