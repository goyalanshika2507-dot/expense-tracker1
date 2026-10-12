#include "ExpenseTracker.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

ExpenseTracker::ExpenseTracker()
    : currentUsername("default"), nextId(1)
{
    loadTransactions();
}

ExpenseTracker::ExpenseTracker(const string& username)
    : currentUsername(username), nextId(1)
{
    loadUserData();
}

ExpenseTracker::~ExpenseTracker()
{
    clearData();
}

void ExpenseTracker::clearData()
{
    for (auto* t : transactions)
    {
        delete t;
    }
    transactions.clear();
    budgets.clear();
    nextId = 1;
}

void ExpenseTracker::setCurrentUser(const string& username)
{
    if (currentUsername != username)
    {
        currentUsername = username;
        clearData();
        loadUserData();
    }
}

string ExpenseTracker::getCurrentUser() const
{
    return currentUsername;
}

void ExpenseTracker::loadUserData()
{
    if (currentUsername.empty()) return;
    string txFile = "data/transactions_" + currentUsername + ".txt";
    if (fs::exists(txFile))
    {
        loadTransactions(txFile);
    }
    else
    {
        loadTransactions("transactions.txt");
    }

    // Load budgets if present
    string bgFile = "data/budgets_" + currentUsername + ".txt";
    if (fs::exists(bgFile))
    {
        ifstream bFile(bgFile);
        string line;
        while (getline(bFile, line))
        {
            stringstream ss(line);
            string cat, limStr;
            if (getline(ss, cat, '|') && getline(ss, limStr))
            {
                try {
                    double lim = stod(limStr);
                    setBudget(cat, lim);
                } catch (...) {}
            }
        }
    }
}

void ExpenseTracker::saveUserData()
{
    if (currentUsername.empty()) return;
    try {
        fs::create_directories("data");
    } catch (...) {}

    string txFile = "data/transactions_" + currentUsername + ".txt";
    saveTransactions(txFile);

    string bgFile = "data/budgets_" + currentUsername + ".txt";
    ofstream bFile(bgFile);
    if (bFile.is_open())
    {
        for (const auto& b : budgets)
        {
            bFile << b.getCategory() << "|" << b.getLimitAmount() << "\n";
        }
        bFile.close();
    }
}

bool ExpenseTracker::addIncome(double amount, const string& date, const string& source)
{
    if (amount <= 0 || date.empty() || source.empty())
    {
        return false;
    }

    transactions.push_back(new Income(nextId++, amount, date, source));
    saveUserData();
    return true;
}

bool ExpenseTracker::addExpense(double amount, const string& date, const string& category)
{
    if (amount <= 0 || date.empty() || category.empty())
    {
        return false;
    }

    transactions.push_back(new Expense(nextId++, amount, date, category));
    saveUserData();
    return true;
}

bool ExpenseTracker::editTransaction(int id, double amount, const string& date, const string& categoryOrSource)
{
    if (amount <= 0 || date.empty() || categoryOrSource.empty())
    {
        return false;
    }

    for (auto* t : transactions)
    {
        if (t && t->getId() == id)
        {
            t->setAmount(amount);
            t->setDate(date);
            t->setCategoryOrSource(categoryOrSource);
            saveUserData();
            return true;
        }
    }
    return false;
}

bool ExpenseTracker::deleteTransaction(int id)
{
    for (auto it = transactions.begin(); it != transactions.end(); ++it)
    {
        if (*it && (*it)->getId() == id)
        {
            delete *it;
            transactions.erase(it);
            saveUserData();
            return true;
        }
    }
    return false;
}

Transaction* ExpenseTracker::findTransactionById(int id) const
{
    for (auto* t : transactions)
    {
        if (t && t->getId() == id)
        {
            return t;
        }
    }
    return nullptr;
}

const vector<Transaction*>& ExpenseTracker::getTransactions() const
{
    return transactions;
}

double ExpenseTracker::getTotalIncome() const
{
    return Reports::calculateTotalIncome(transactions);
}

double ExpenseTracker::getTotalExpense() const
{
    return Reports::calculateTotalExpense(transactions);
}

double ExpenseTracker::getBalance() const
{
    return Reports::calculateBalance(transactions);
}

const vector<Budget>& ExpenseTracker::getBudgets() const
{
    return budgets;
}

void ExpenseTracker::setBudget(const string& category, double limit)
{
    for (auto& b : budgets)
    {
        if (b.getCategory() == category)
        {
            b.setLimitAmount(limit);
            saveUserData();
            return;
        }
    }
    budgets.emplace_back(category, limit);
    saveUserData();
}

Budget* ExpenseTracker::getBudgetForCategory(const string& category)
{
    for (auto& b : budgets)
    {
        if (b.getCategory() == category)
        {
            return &b;
        }
    }
    return nullptr;
}

const Budget* ExpenseTracker::getBudgetForCategory(const string& category) const
{
    for (const auto& b : budgets)
    {
        if (b.getCategory() == category)
        {
            return &b;
        }
    }
    return nullptr;
}

// Console operations preserved from Garima's original expensetracker.cpp
void ExpenseTracker::addIncomeConsole()
{
    double amount;
    string source, date;

    cout << "Enter income amount: ";
    if (!(cin >> amount) || amount <= 0)
    {
        cout << "Invalid amount!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter source/category: ";
    getline(cin, source);

    cout << "Enter date (DD/MM/YYYY): ";
    getline(cin, date);

    if (addIncome(amount, date, source))
    {
        cout << "Income added successfully!" << endl;
    }
    else
    {
        cout << "Failed to add income: invalid input." << endl;
    }
}

void ExpenseTracker::addExpenseConsole()
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

    if (addExpense(amount, date, category))
    {
        cout << "Expense added successfully!" << endl;
    }
    else
    {
        cout << "Failed to add expense: invalid input." << endl;
    }
}

void ExpenseTracker::viewHistory()
{
    if (transactions.empty())
    {
        cout << "No transactions available!" << endl;
        return;
    }

    cout << "\nID\tType\tAmount\tCategory/Source\tDate\n";
    for (auto* t : transactions)
    {
        if (t) t->display();
    }
}

void ExpenseTracker::calculateBalance()
{
    cout << fixed << setprecision(2);
    cout << "\nTotal Income: Rs. " << getTotalIncome() << endl;
    cout << "Total Expenses: Rs. " << getTotalExpense() << endl;
    cout << "Current Balance: Rs. " << getBalance() << endl;
}

void ExpenseTracker::searchTransactionConsole()
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

    Transaction* t = findTransactionById(searchId);
    if (t)
    {
        cout << "\nTransaction found:\n";
        t->display();
    }
    else
    {
        cout << "Transaction not found!" << endl;
    }
}

void ExpenseTracker::editTransactionConsole()
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

    Transaction* t = findTransactionById(editId);
    if (!t)
    {
        cout << "Transaction not found!" << endl;
        return;
    }

    double amount;
    string catOrSrc, date;

    cout << "Enter new amount: ";
    if (!(cin >> amount) || amount <= 0)
    {
        cout << "Invalid amount!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter new category/source: ";
    getline(cin, catOrSrc);

    cout << "Enter new date: ";
    getline(cin, date);

    if (editTransaction(editId, amount, date, catOrSrc))
    {
        cout << "Transaction updated successfully!" << endl;
    }
    else
    {
        cout << "Failed to update transaction." << endl;
    }
}

void ExpenseTracker::deleteTransactionConsole()
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

    if (deleteTransaction(deleteId))
    {
        cout << "Transaction deleted successfully!" << endl;
    }
    else
    {
        cout << "Transaction not found!" << endl;
    }
}

void ExpenseTracker::saveTransactions(const string& filename)
{
    ofstream file(filename);
    if (!file)
    {
        return;
    }

    for (const auto* t : transactions)
    {
        if (t)
        {
            file << t->getId() << "|"
                 << t->getType() << "|"
                 << fixed << setprecision(2) << t->getAmount() << "|"
                 << t->getCategoryOrSource() << "|"
                 << t->getDate() << "\n";
        }
    }
    file.close();
}

void ExpenseTracker::loadTransactions(const string& filename)
{
    ifstream file(filename);
    if (!file)
    {
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty()) continue;
        stringstream ss(line);
        string idText, type, amountText, catOrSrc, date;

        getline(ss, idText, '|');
        getline(ss, type, '|');
        getline(ss, amountText, '|');
        getline(ss, catOrSrc, '|');
        getline(ss, date);

        try
        {
            int id = stoi(idText);
            double amount = stod(amountText);

            if (id <= 0 || amount <= 0 || catOrSrc.empty() || date.empty())
            {
                continue;
            }

            if (id >= nextId)
            {
                nextId = id + 1;
            }

            if (type == "Income")
            {
                transactions.push_back(new Income(id, amount, date, catOrSrc));
            }
            else if (type == "Expense")
            {
                transactions.push_back(new Expense(id, amount, date, catOrSrc));
            }
        }
        catch (...)
        {
            // Skip invalid line
        }
    }
    file.close();
}
