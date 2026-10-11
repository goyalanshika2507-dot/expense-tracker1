#include "ExpenseTracker.h"
#include "Income.h"
#include "Expense.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>

using namespace std;

ExpenseTracker::ExpenseTracker()
{
nextId = 1;
loadTransactions();
}

// Add income
void ExpenseTracker::addIncome()
{
double amount;
string category, date;


cout << "Enter income amount: ";
if (!(cin >> amount) || amount <= 0)
{
    cout << "Invalid amount!" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return;
}

cin.ignore(numeric_limits<streamsize>::max(), '\n');

cout << "Enter category: ";
getline(cin, category);

cout << "Enter date (DD/MM/YYYY): ";
getline(cin, date);

if (category.empty() || date.empty())
{
    cout << "Category and date cannot be empty!" << endl;
    return;
}

transactions.push_back(
    new Income(nextId++, amount, category, date)
);

saveTransactions();

cout << "Income added successfully!" << endl;


}

// Add expense
void ExpenseTracker::addExpense()
{
double amount;
string category, date;


cout << "Enter expense amount: ";
if (!(cin >> amount) || amount <= 0)
{
    cout << "Invalid amount!" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return;
}

cin.ignore(numeric_limits<streamsize>::max(), '\n');

cout << "Enter category: ";
getline(cin, category);

cout << "Enter date (DD/MM/YYYY): ";
getline(cin, date);

if (category.empty() || date.empty())
{
    cout << "Category and date cannot be empty!" << endl;
    return;
}

transactions.push_back(
    new Expense(nextId++, amount, category, date)
);

saveTransactions();

cout << "Expense added successfully!" << endl;


}

// View transaction history
void ExpenseTracker::viewHistory()
{
if (transactions.empty())
{
cout << "No transactions available!" << endl;
return;
}


cout << "\nID\tType\tAmount\tCategory\tDate\n";

for (int i = 0; i < transactions.size(); i++)
{
    transactions[i]->display();
}


}

// Calculate balance
void ExpenseTracker::calculateBalance()
{
double totalIncome = 0;
double totalExpense = 0;


for (int i = 0; i < transactions.size(); i++)
{
    if (transactions[i]->getType() == "Income")
    {
        totalIncome += transactions[i]->getAmount();
    }
    else
    {
        totalExpense += transactions[i]->getAmount();
    }
}

cout << fixed << setprecision(2);

cout << "\nTotal Income: Rs. " << totalIncome << endl;
cout << "Total Expenses: Rs. " << totalExpense << endl;
cout << "Current Balance: Rs. "
     << totalIncome - totalExpense << endl;


}

// Search transaction by ID
void ExpenseTracker::searchTransaction()
{
int searchId;


cout << "Enter transaction ID: ";

if (!(cin >> searchId))
{
    cout << "Invalid ID!" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return;
}

for (int i = 0; i < transactions.size(); i++)
{
    if (transactions[i]->getId() == searchId)
    {
        cout << "\nTransaction found:\n";
        cout << "ID\tType\tAmount\tCategory\tDate\n";

        transactions[i]->display();
        return;
    }
}

cout << "Transaction not found!" << endl;


}

// Edit transaction
void ExpenseTracker::editTransaction()
{
int editId;


cout << "Enter transaction ID to edit: ";

if (!(cin >> editId))
{
    cout << "Invalid ID!" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return;
}

for (int i = 0; i < transactions.size(); i++)
{
    if (transactions[i]->getId() == editId)
    {
        double amount;
        string category, date;

        cout << "Enter new amount: ";

        if (!(cin >> amount) || amount <= 0)
        {
            cout << "Invalid amount!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter new category: ";
        getline(cin, category);

        cout << "Enter new date: ";
        getline(cin, date);

        if (category.empty() || date.empty())
        {
            cout << "Category and date cannot be empty!" << endl;
            return;
        }

        transactions[i]->setAmount(amount);
        transactions[i]->setCategory(category);
        transactions[i]->setDate(date);

        saveTransactions();

        cout << "Transaction updated successfully!" << endl;
        return;
    }
}

cout << "Transaction not found!" << endl;


}

// Delete transaction
void ExpenseTracker::deleteTransaction()
{
int deleteId;


cout << "Enter transaction ID to delete: ";

if (!(cin >> deleteId))
{
    cout << "Invalid ID!" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return;
}

for (int i = 0; i < transactions.size(); i++)
{
    if (transactions[i]->getId() == deleteId)
    {
        delete transactions[i];

        transactions.erase(transactions.begin() + i);

        saveTransactions();

        cout << "Transaction deleted successfully!" << endl;
        return;
    }
}

cout << "Transaction not found!" << endl;


}

// Save transactions to file
void ExpenseTracker::saveTransactions()
{
ofstream file("transactions.txt");


if (!file)
{
    cout << "Error saving transactions!" << endl;
    return;
}

for (int i = 0; i < transactions.size(); i++)
{
    file << transactions[i]->getId() << "|"
         << transactions[i]->getType() << "|"
         << transactions[i]->getAmount() << "|"
         << transactions[i]->getCategory() << "|"
         << transactions[i]->getDate() << "\n";
}

file.close();
 

}

// Load transactions from file
void ExpenseTracker::loadTransactions()
{
ifstream file("transactions.txt");

 
if (!file)
{
    return;
}

string line;

while (getline(file, line))
{
    stringstream ss(line);

    string idText, type, amountText, category, date;

    getline(ss, idText, '|');
    getline(ss, type, '|');
    getline(ss, amountText, '|');
    getline(ss, category, '|');
    getline(ss, date);

    try
    {
        size_t usedId, usedAmount;

        int id = stoi(idText, &usedId);
        double amount = stod(amountText, &usedAmount);

        if (usedId != idText.size() ||
            usedAmount != amountText.size() ||
            id <= 0 || amount <= 0)
        {
            continue;
        }

        if (type != "Income" && type != "Expense")
        {
            continue;
        }

        if (category.empty() || date.empty())
        {
            continue;
        }

        if (id >= nextId)
        {
            nextId = id + 1;
        }

        if (type == "Income")
        {
            transactions.push_back(
                new Income(id, amount, category, date)
            );
        }
        else
        {
            transactions.push_back(
                new Expense(id, amount, category, date)
            );
        }
    }
    catch (...)
    {
        cout << "Skipping an invalid transaction record." << endl;
    }
}

file.close();

}

// Destructor
ExpenseTracker::~ExpenseTracker()
{
for (int i = 0; i < transactions.size(); i++)
{
delete transactions[i];
}

transactions.clear();


}
