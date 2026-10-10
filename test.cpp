#include <iostream>
#include <vector>

#include "Income.h"
#include "Expense.h"

using namespace std;

int main()
{
    vector<Transaction*> transactions;

    Income* i1 = new Income(101, 15000, "10-10-2026", "Salary");

    Expense* e1 = new Expense(102, 500, "10-10-2026", "Food");

    Expense* e2 = new Expense(103, 1000, "10-10-2026", "Travel");

    Income* i2 = new Income(104, 2000, "10-10-2026", "Freelancing");

    transactions.push_back(i1);
    transactions.push_back(e1);
    transactions.push_back(e2);
    transactions.push_back(i2);

    cout << "===== ALL TRANSACTIONS =====" << endl;

    for (Transaction* t : transactions)
    {
        t->display();
        cout << endl;
    }

    for (Transaction* t : transactions)
    {
        delete t;
    }

    return 0;
}