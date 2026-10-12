#ifndef EXPENSETRACKER_H
#define EXPENSETRACKER_H

#include "Transaction.h"
#include "Income.h"
#include "Expense.h"
#include "Budget.h"
#include "Reports.h"
#include <vector>
#include <string>

using namespace std;

class ExpenseTracker
{
private:
    string currentUsername;
    vector<Transaction*> transactions;
    vector<Budget> budgets;
    int nextId;

public:
    ExpenseTracker();
    explicit ExpenseTracker(const string& username);
    ~ExpenseTracker();

    // User session management & persistence
    void setCurrentUser(const string& username);
    string getCurrentUser() const;
    void loadUserData();
    void saveUserData();
    void clearData();

    // Programmatic transaction operations
    bool addIncome(double amount, const string& date, const string& source);
    bool addExpense(double amount, const string& date, const string& category);
    bool editTransaction(int id, double amount, const string& date, const string& categoryOrSource);
    bool deleteTransaction(int id);

    Transaction* findTransactionById(int id) const;
    const vector<Transaction*>& getTransactions() const;

    // Financial summaries (delegated to Reports module)
    double getTotalIncome() const;
    double getTotalExpense() const;
    double getBalance() const;

    // Budget operations
    const vector<Budget>& getBudgets() const;
    void setBudget(const string& category, double limit);
    Budget* getBudgetForCategory(const string& category);
    const Budget* getBudgetForCategory(const string& category) const;

    // Console methods (preserved from Garima's original implementation)
    void addIncomeConsole();
    void addExpenseConsole();
    void viewHistory();
    void calculateBalance();
    void searchTransactionConsole();
    void editTransactionConsole();
    void deleteTransactionConsole();

    // Direct file operations
    void saveTransactions(const string& filename = "transactions.txt");
    void loadTransactions(const string& filename = "transactions.txt");

    // Backward-compatibility aliases for console interface
    void addIncome() { addIncomeConsole(); }
    void addExpense() { addExpenseConsole(); }
    void searchTransaction() { searchTransactionConsole(); }
    void editTransaction() { editTransactionConsole(); }
    void deleteTransaction() { deleteTransactionConsole(); }
};

#endif
