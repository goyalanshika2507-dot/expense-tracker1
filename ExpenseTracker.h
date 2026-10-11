#ifndef EXPENSETRACKER_H
#define EXPENSETRACKER_H

#include "Transaction.h"
#include <vector>
#include <string>
using namespace std;

class ExpenseTracker
{
private:
vector<Transaction*> transactions;
int nextId;


void saveTransactions();
void loadTransactions();


public:
ExpenseTracker();


void addIncome();
void addExpense();

void viewHistory();
void calculateBalance();

void searchTransaction();
void editTransaction();
void deleteTransaction();

~ExpenseTracker();


};

#endif
